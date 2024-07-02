#include "y/utest/run.hpp"
#include "y/container/cxx/array.hpp"
#include <cmath>
#include "y/mkl/ode/rk45/scheme.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/api.hpp"
#include "y/mkl/root/zbis.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;
using namespace MKL;

static const unsigned H  = 1;
static const unsigned W  = 2;
static const unsigned Na = 3;
static const unsigned Cl = 4;

static const unsigned NVAR = 4;
static const double   Dc[NVAR+1] =
{
    0,
    9.31*1e-9,
    5.27*1e-9,
    1.33*1e-9,
    2.03*1e-9
};

typedef CxxArray<double> Array;

static double       pH0 = 5.5;
static double       Osm0 = 0.1;

static double       pH1 = 7.2;
static double       Osm1 = 0.04;

static const double Kw  = 1e-14;
static double       L   = 1e-3;



class Reactor
{
public:
    explicit Reactor() :
    eq(this, & Reactor::compute),
    cb(this, & Reactor::callback),
    Z(NVAR),
    C0(NVAR),
    C1(NVAR),
    C_m(NVAR),
    C_p(NVAR),
    omega(NVAR),
    F(NVAR),
    dC1(NVAR)
    {
        Z[H]   = 1;
        Z[W]  = -1;
        Z[Na] =  1;
        Z[Cl] = -1;

        C0[H] = pow(10,-pH0);
        C0[W] = Kw / C0[H];

        C0[Na] = Osm0 / 2 - C0[H];
        C0[Cl] = Osm0 / 2 - C0[W];

        C1[H] = pow(10,-pH1);
        C1[W] = Kw / C1[H];

        C1[Na] = Osm1 / 2 - C1[H];
        C1[Cl] = Osm1 / 2 - C1[W];
    }


    ODE::Named<double>::Equation eq;
    ODE::Named<double>::Callback cb;
    Array Z;
    Array C0;
    Array C1;
    Array C_m;
    Array C_p;
    Array omega;
    Array F;
    Array dC1;

    void computeOmega(const Readable<double> &C)
    {
        double Omega_m  = 0;
        double Omega_p  = 0;
        for(size_t i=1;i<=NVAR;++i)
        {
            C_m[i]   = (C0[i]+C[i])/2;
            C_p[i]   = (C[i]+C1[i])/2;
            Omega_m += (Z[i]*Z[i]) * Dc[i] * C_m[i];
            Omega_p += (Z[i]*Z[i]) * Dc[i] * C_p[i];
        }
        const double den = Omega_m*Omega_m + Omega_p * Omega_p;
        for(size_t i=1;i<=NVAR;++i)
        {
            omega[i] = Z[i] * Dc[i] * (Omega_m * C_m[i] + Omega_p * C_p[i]) / den;
        }
    }

    double computeF(const Readable<double> &C)
    {
        double xs = 0;
        for(size_t i=1;i<=NVAR;++i)
        {
            F[i] = Dc[i] * (C0[i] - 2*C[i] + C1[i])/(L*L);
            xs += Z[i] * F[i];
        }
        return xs;
    }

    inline void compute(Writable<double> &dCdt,double,const Readable<double> &C)
    {
        computeOmega(C);
        const double ZF = computeF(C);
        for(size_t i=1;i<=NVAR;++i)
        {
            dCdt[i] = dC1[i] = F[i] - omega[i] * ZF;
        }
        //std::cerr << "dC=" << dC1 << std::endl;
        const double h = C[H];
        const double w = C[W];
        const double wph = w+h;
        dCdt[H] = h * (dC1[H] - dC1[W])/wph;
        dCdt[W] = w * (dC1[W] - dC1[H])/wph;
        
    }

    struct Water
    {
        double h0;
        double w0;

        double operator()(const double xi)
        {
            return Kw - (h0+xi)*(w0+xi);
        }
    };

    void callback(double, Writable<double> &C) const
    {
        ZBis<double> zfind;
        const double h0 = C[H];
        const double w0 = C[W];
        Water        f = { h0, w0 };
        Triplet<double> xx = { 0,   0, 0 };
        Triplet<double> ff = { f(0),0, 0 };
        if(ff.a<=0)
        {
            xx.c = Max(-h0,-w0);
            ff.c = f(xx.c);
        }
        else
        {
            assert(ff.a>0);
            xx.c = sqrt(Kw);
            ff.c = f(xx.c);
        }
        //std::cerr << "xx=" << xx << " -> " << ff << std::endl;
        zfind(f,xx,ff);
        const double xi = xx.b;
        //std::cerr << " => xi=" << xx.b << std::endl;
        C[W] = Kw/(C[H] += xi);
    }

    inline void save(OutputFile &fp, const double t, const Readable<double> &C) const
    {
        fp("%.15g",t);
        for(size_t i=1;i<=NVAR;++i)
        {
            fp(" %.15g", C[i]);
        }
        {
            double xs = 0;
            for(size_t i=1;i<=NVAR;++i) xs += Z[i] * C[i];
            fp(" %.15g", xs);
        }
        fp << '\n';
        fp.flush();
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(Reactor);
};



Y_UTEST(diff)
{
    Reactor rc;
    std::cerr << "C0=" << rc.C0 << std::endl;
    std::cerr << "C1=" << rc.C1 << std::endl;

    Array C(NVAR);

    C[H] = C[W] = sqrt(Kw);

    ODE::RK45::Scheme<double> scheme(NVAR, 1e-7);
    rc.computeOmega(C);
    std::cerr << "omega=" << rc.omega << std::endl;

    std::cerr << "C=" << C << std::endl;

    OutputFile fp("diff.dat");
    rc.save(fp,0,C);

    double dt     = 0.1;
    double dt_ini = dt/100;
    for(size_t i=0;i<=100000;++i)
    {
        const double t0 = i*dt;
        const double t1 = (i+1)*dt;
        scheme(C,t0,t1,dt_ini,rc.eq,& rc.cb);
        std::cerr << "C=" << C << std::endl;
        rc.save(fp,t1,C);
    }
    std::cerr << "C0=" << rc.C0 << std::endl;
    std::cerr << "C1=" << rc.C1 << std::endl;
    std::cerr << "Z="  << rc.Z << std::endl;
}
Y_UDONE()

