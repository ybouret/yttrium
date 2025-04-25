

#include "y/chemical/plexus/reactors.hpp"
//#include "y/chemical/plexus/conservation/wardens.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"


 
using namespace Yttrium;
using namespace Chemical;


static const char *Description[] =
{
    "%water",
    "%fluorescein",
    "Q^-"
};

static const xreal_t lam_AH = 0.55;
static const xreal_t lam_Am = 1.0 - lam_AH;


struct ZeroH
{
    const xreal_t Kw, Ka, Lam, Q;

    inline xreal_t operator()(xreal_t h) const
    {
        return h - Kw/h - Ka/(lam_AH * h + lam_Am * Ka) * Lam + Q;
    }
};


Y_UTEST(fluo)
{

    Random::MT19937 ran;
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";


    Library       lib;
    Equilibria    eqs;
    Repertory     rep;
    const xreal_t dQ = ran.to<double>() * 1e-3;

    for(unsigned i=0;i<sizeof(Description)/sizeof(Description[0]);++i)
    {
        const char * const data = Description[i];
        weasel(lib,eqs,rep,Lingo::Module::Open(data,data) );
    }

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters cls(xml,eqs,0.0);
    cls.graphViz("fluo");

    const size_t M  = lib->size();
    const size_t np = 100;
    XMatrix      C(M+1,np);



    //XWritable &h = lib("H^+")(C,TopLevel);
    const Equilibrium &fluo  = eqs["fluorescein"];
    const Equilibrium &water = eqs["water"];

    const xreal_t &Ka  = fluo(cls.K,TopLevel);
    const xreal_t &Kw  = water(cls.K,TopLevel);
    const xreal_t  pH0 = 6;
    const xreal_t  pH1 = 8;
    const xreal_t  Cf  = 0.0001;

    const xreal_t h0  = pow(10.0,-real_t(pH0));
    const xreal_t h1  = pow(10.0,-real_t(pH1));

    const xreal_t AH0 = Cf * h0 / (Ka+h0);
    const xreal_t Am0 = Cf - AH0;

    const xreal_t AH1 = Cf * h1 / (Ka+h1);
    const xreal_t Am1 = Cf - AH1;

    const xreal_t w0  = Kw/h0;
    const xreal_t w1  = Kw/h1;

    const xreal_t q0  = w0+Am0-h0;
    const xreal_t q1  = w1+Am1-h1;




    XWritable &Lam = C[M+1];
    XWritable &Q   = lib("Q^-")(C,TopLevel);
    XWritable &h   = lib("H^+")(C,TopLevel);
    XWritable &w   = lib("HO^-")(C,TopLevel);


    Lam[1]  = lam_AH * AH0 + lam_Am * Am0;
    Lam[np] = lam_AH * AH1 + lam_Am * Am1;
    Q[1]    = q0;
    Q[np]   = q1;

    h[1]  = h0; w[1]  = w0;
    h[np] = h1; w[np] = w1;


    MKL::ZBis<xreal_t> solve;
    for(size_t i=2;i<np;++i)
    {
        const xreal_t u = xreal_t(i-1)/xreal_t(np);
        const xreal_t v = 1.0 - u;
        Lam[i] = u*Lam[np]+ v* Lam[1];
        Q[i]   = u*Q[np]  + v* Q[1];

        struct   ZeroH F = { Kw, Ka, Lam[i], Q[i] };
        XTriplet hh    = { h0, 0, h1 };
        XTriplet zh    = { F(hh.a), 0, F(hh.c) };
        //std::cerr << "zh=" << zh << std::endl;
        //solve(F,hh,zh);

        solve(F,hh,zh);
        h[i] = hh.b;
        w[i] = Kw/h[i];

    }



    OutputFile fp("profile.dat");
    fp("#u pH\n");
    for(size_t i=1;i<=np;++i)
    {
        const xreal_t u = xreal_t(i-1)/xreal_t(np);
        fp("%.15g %.15g\n", real_t(u), -log10( real_t(h[i])) );
    }






}
Y_UDONE()
