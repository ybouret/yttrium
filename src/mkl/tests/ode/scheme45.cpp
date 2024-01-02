#include "y/mkl/ode/rk45/cash-karp.hpp"
#include "y/mkl/ode/rk45/controller.hpp"
#include "y/mkl/ode/integrator.hpp"

#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/container/cxx/array.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/cxx-identifier.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    struct Meca
    {
        T mass;
        T k;
        T lam;

        inline void compute(Writable<T> &dydt,T,const Readable<T> &y)
        {
            const T x     = y[1];
            const T v     = y[2];
            const T force = -k*x - lam*v;

            T &dxdt = dydt[1]; dxdt = v;
            T &dvdt = dydt[2]; dvdt = force/mass;

        }
    };


    template <typename T> static inline
    void testScheme(Random::Bits &)
    {
        static const String &tid = RTTI::Name<T>();

        std::cerr << "Enter Scheme<" << tid << ">" << std::endl;
        typename ODE::RK45::Step<T>::Pointer step = new ODE::RK45::CashKarp<T>();
        ODE::RK45::Controller<T>             rk45(step);
        ODE::Integrator<T>                   odeint(1e-7);

        Meca<T>                          meca = { 1.314f, 1.01, 0.2f };
        typename ODE::Named<T>::Equation drvs( &meca, & Meca<T>::compute );

        CxxArray<T> y(2);

        y[1] = 0.1f;
        y[2] = 0.0f;

        const String     fileName = "meca-" + CxxIdentifier::From(tid) + ".dat";
        Libc::OutputFile fp(fileName);
        fp("0 %.15g\n",double(y[1]));
        const T      tmax = 60;
        const size_t np   = 100;;
        const T      hmin(0.01);
        for(size_t i=1;i<=np;++i)
        {
            const T x0 = (tmax*(i-1))/np;
            const T x1 = (tmax*(i))/np;
            T       h  = hmin;
            odeint.run(y,x0,x1,h,drvs,NULL,rk45);
            fp("%.15g %.15g\n",double(x1),double(y[1]));
        }


        std::cerr << "Leave Scheme<" << tid << ">" << std::endl << std::endl;



    }
}

Y_UTEST(ode_scheme45)
{
    Random::Rand ran;

    testScheme<float>(ran);
    testScheme<double>(ran);
    testScheme<long double>(ran);


}
Y_UDONE()
