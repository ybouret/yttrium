
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
            dydt[1] = k * y[1];
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(dExp);
    };

}


Y_UTEST(odeint)
{

    dExp<double> dd(-1.2);
    ODE::Named<double>::Equation   drvs( &dd, & dExp<double>::compute );
    ODE::RK45_Step<double>::Handle step = new ODE::CashKarp<double>();
    ODE::RK45<double>              rk45(step);
    ODE::Integrator<double>        odeint(1e-7);
    

}
Y_UDONE()

