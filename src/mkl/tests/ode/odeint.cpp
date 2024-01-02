
#include "y/mkl/ode/integrator.hpp"
#include "y/mkl/ode/rk45/cash-karp.hpp"
#include "y/mkl/ode/rk45/controller.hpp"

#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/api.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    class dExp
    {
    public:
        explicit dExp(const T fac) noexcept : k(fac) {}
        virtual ~dExp() noexcept {}

        const T k;

        inline void compute(Writable<T> &dydt, double, const Readable<T> &y)
        {
            dydt[1] = -Fabs<T>::Of(k) * y[1];
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(dExp);
    };

    struct LotkaVolterra
    {
        double alpha, beta, gamma, delta;

        inline void compute(Writable<double> &dFdt, double, const Readable<double> &F)
        {
            const double x = F[1];
            const double y = F[2];

            dFdt[1] = alpha * x - beta * x * y;
            dFdt[2] = delta * x * y - gamma * y;

        }

    };

}


Y_UTEST(odeint)
{

    dExp<double>                     dd(1.2);
    ODE::RK45::Step<double>::Pointer step = new ODE::RK45::CashKarp<double>();
    ODE::RK45::Controller<double>    rk45(step);
    ODE::Integrator<double>          odeint(1e-7);

    if(true)
    {
        ODE::Named<double>::Equation drvs( &dd, & dExp<double>::compute );

        Vector<double> y(1,2.1);
        double         h    = 1e-4;
        const double   xini = 0;
        const double   xend = 10;
        const size_t   np   = 100;

        Libc::OutputFile fp("exp.dat");
        fp("%.15g %.15g\n",xini,y[1]);
        for(size_t i=1;i<=np;++i)
        {
            const double x1 = xini + (i-1)*(xend-xini)/double(np);
            const double x2 = xini + i*(xend-xini)/double(np);
            odeint.run(y, x1, x2, h, drvs, 0, rk45);
            fp("%.15g %.15g\n",x2,y[1]);
        }
    }

    {
        LotkaVolterra lv = { 2.0/3, 4.0/3, 1.0, 1.0 };
        ODE::Named<double>::Equation drvs( &lv, &LotkaVolterra::compute );

        Vector<double> F(2,1);


        double         h    = 1e-4;
        const double   xini = 0;
        const double   xend = 10;
        const size_t   np   = 100;

        Libc::OutputFile fp("lv.dat");
        fp("%.15g %.15g %.15g\n",xini,F[1], F[2]);
        for(size_t i=1;i<=np;++i)
        {
            const double x1 = xini + (i-1)*(xend-xini)/double(np);
            const double x2 = xini + i*(xend-xini)/double(np);
            odeint.run(F, x1, x2, h, drvs, 0, rk45);
            fp("%.15g %.15g %.15g\n",x2,F[1], F[2]);
        }

    }







}
Y_UDONE()

