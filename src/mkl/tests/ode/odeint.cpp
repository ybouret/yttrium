
#include "y/mkl/ode/integrator.hpp"
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

}


Y_UTEST(odeint)
{

    dExp<double>                   dd(1.2);
    ODE::Named<double>::Equation   drvs( &dd, & dExp<double>::compute );
    ODE::RK45_Step<double>::Handle step = new ODE::CashKarp<double>();
    ODE::RK45<double>              rk45(step);
    ODE::Integrator<double>        odeint(1e-7);

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
Y_UDONE()

