#include "y/mkl/ode/rk45/scheme.hpp"
#include "y/mkl/ode/rk45/cash-karp.hpp"
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
            T &dvdt = dydt[1]; dvdt = force/mass;

        }
    };


    template <typename T> static inline
    void testScheme(Random::Bits &)
    {
        static const String &tid = RTTI::Name<T>();

        std::cerr << "Enter Scheme<" << tid << ">" << std::endl;
        ODE::RK45::Scheme<ODE::RK45::CashKarp,T> scheme(2);

        Meca<T>                          meca = { 0.11f, 1.01, 0.0f };
        typename ODE::Named<T>::Equation diffeq( &meca, & Meca<T>::compute );

        CxxArray<T> y(2);

        y[1] = 0.1f;
        y[2] = 0.0f;

        const String     fileName = "meca-" + CxxIdentifier::From(tid) + ".dat";
        Libc::OutputFile fp(fileName);

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
