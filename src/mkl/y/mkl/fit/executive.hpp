//! \file

#ifndef Y_Fit_Executive_Included
#define Y_Fit_Executive_Included 1

#include "y/mkl/fit/step-inventor.hpp"
#include "y/mkl/fit/least-squares/roll.hpp"
#include "y/mkl/numeric.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            //__________________________________________________________________
            //
            //
            //
            //! Managing fit memory and procedure
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA,typename ORDINATE>
            class Executive : public Proxy< const LeastSquares<ABSCISSA,ORDINATE> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef LeastSquaresRoll<ABSCISSA,ORDINATE> RollType;         //!< alias
                typedef LeastSquares<ABSCISSA,ORDINATE>     LeastSquaresType; //!< alias
                typedef StepInventor<ABSCISSA>              StepInventorType; //!< alias
                typedef Sample<ABSCISSA,ORDINATE>           SampleType;       //!< alias
                typedef Samples<ABSCISSA,ORDINATE>          SamplesType;      //!< alias
                typedef Proxy<const LeastSquaresType>       ProxyType;        //!< alias
                typedef Domain<ABSCISSA>                    DomainType;       //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit Executive(const bool verb=true) :
                ProxyType(),
                mine( new LeastSquaresType() ),
                roll( new RollType()         ),
                solv( new StepInventorType() ),
                verbose(verb)
                {}

                //! cleanup
                inline virtual ~Executive() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

#include "executive/d2.hpp"

                template <
                typename FUNCTION,
                typename GRADIENT,
                typename SAMPLE>
                bool run(FUNCTION           &F,
                         SAMPLE             &S,
                         Writable<ABSCISSA> &aorg,
                         const DomainType   &adom,
                         const Booleans     &used,
                         GRADIENT           &G)
                {

                    static const ABSCISSA zero = 0;
                    static const ABSCISSA one  = 1;

                    static const ABSCISSA tol = Numeric<ABSCISSA>::SQRT_EPSILON;
                    assert( aorg.size() == used.size() ) ;
                    assert( aorg.size() == adom.size() );
                    assert( adom.contains(aorg) );

                    //__________________________________________________________
                    //
                    // top level initialization
                    //__________________________________________________________
                    ABSCISSA                   Dorg = D2(F,S,aorg,used,G); // full metrics
                    Writable<ABSCISSA>       & atry = solv->atry;          // alias
                    const Readable<ABSCISSA> & beta = mine->beta;          // alias
                    const Readable<ABSCISSA> & step = solv->step;          // alias
                    Writable<ABSCISSA>       & atmp = solv->atmp;          // alias
                    const int                  pmin = solv->pmin;          // alias
                    const int                  pmax = solv->pmax;          // alias
                    const size_t               nvar = aorg.size();         // num variables
                    int                 p    = -4;                         // initial guess TODO
                    solv->prepare(nvar);                                   // workspace

                    D2Call<FUNCTION,SAMPLE> H = { nvar, aorg, atry, atmp, F,S, *this };

                    Libc::OutputFile fp("D2.dat");

                    unsigned long cycle = 0;
                CYCLE:
                    const int p0 = p;
                    ++cycle;
                    Y_MKL_FIT("-------- cycle = " << cycle << " --------");
                    Y_MKL_FIT("Dorg  = " << Dorg << "# @" << aorg << ", p=" << p);
                    Y_MKL_FIT("beta  = " << beta);

                    fp("%lu %.15g\n", cycle, double(Dorg) ).flush();

                    //----------------------------------------------------------
                    //
                    // compute predicted step
                    //
                    //----------------------------------------------------------
                BUILD_STEP:
                    if(!solv->buildStep(*mine,aorg,adom,p,used,verbose))
                    {
                        Y_MKL_FIT("-- no possible step");
                        return false;
                    }

                    //----------------------------------------------------------
                    //
                    // here, we have an approximated step
                    //
                    //----------------------------------------------------------

                    const ABSCISSA Dtry = D2(F,S,atry);
                    Y_MKL_FIT("Dtry  = " << Dtry << "# @" << atry << ", p=" << p);

                    const ABSCISSA sigma = mine->dot(beta,step);
                    const ABSCISSA gamma = mine->quad(solv->hess,step,solv->atmp);
                    Y_MKL_FIT("sigma = " << sigma);
                    Y_MKL_FIT("gamma = " << gamma);
                    //const ABSCISSA gamma = Dtry-Dorg+sigma;


                    {
                        const String     fn = Formatted::Get("d2-%lu.dat",cycle);
                        Libc::OutputFile d2(fn);
                        const size_t     nn = 100;
                        for(size_t i=0;i<=nn;++i)
                        {
                            const ABSCISSA u = ABSCISSA(i) / ABSCISSA(nn);
                            d2("%.15g %.15g %.15g\n", double(u), double(H(u)), double(Dorg-sigma*u));
                        }
                    }

                    if(Dtry>Dorg)
                    {
                        Y_MKL_FIT("Bad!");
                        std::cerr << "sigma=" << sigma << std::endl;
                        exit(0);
                        Y_MKL_FIT_DEGRADE();
                        goto BUILD_STEP;
                    }

                    Y_MKL_FIT("-- accepted!");
                    const bool kept      = (p==p0);
                    if(kept)
                    {
                        Y_MKL_FIT("-- upgrade parameter");
                        if(--p<=pmin) p = pmin;
                        assert(Dtry<=Dorg);
                        const ABSCISSA del = Dorg-Dtry;
                        const ABSCISSA sum = Dorg+Dtry;
                        const ABSCISSA lim = sum * tol;
                        std::cerr << "delta=" << del << " / limit=" << lim << std::endl;
                        for(size_t i=1;i<=nvar;++i)
                        {
                            if(!used[i]) continue;
                            std::cerr << "aorg[" << i << "]=" << aorg[i] << " + " << (atry[i]-aorg[i]) << std::endl;
                        }


                    }

                    Tao::Load(aorg,atry);
                    Dorg = D2(F,S,aorg,used,G);


                    //----------------------------------------------------------
                    // prepare for next cycle
                    //----------------------------------------------------------


                    if(cycle<=4)
                        goto CYCLE;


                    return false;
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
                AutoPtr<LeastSquaresType> mine;
                AutoPtr<RollType>         roll;
                AutoPtr<StepInventorType> solv;
                inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept { return *mine; }

                template <typename FUNCTION, typename SAMPLE>
                struct D2Call
                {
                    size_t                    nvar;
                    const Readable<ABSCISSA> &aorg;
                    const Readable<ABSCISSA> &atry;
                    Writable<ABSCISSA>       &atmp;
                    FUNCTION                 &F;
                    SAMPLE                   &S;
                    Executive                &self;

                    inline ABSCISSA operator()(const ABSCISSA u)
                    {
                        static const ABSCISSA _1(1);
                        const ABSCISSA v = _1 - u;
                        for(size_t i=nvar;i>0;--i)
                        {
                            atmp[i] = v * aorg[i] + u * atry[i];
                        }
                        return self.D2(F,S,atmp);
                    }

                };


            public:
                bool verbose; //!< verbosity
            };
        }

    }

}

#endif

