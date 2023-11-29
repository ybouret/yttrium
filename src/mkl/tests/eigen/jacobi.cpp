#include "y/mkl/eigen/jacobi.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline void doJacobi(Random::Bits & )
    {
        const String &ts = RTTI::Name<T>();
        std::cerr << "Jacobi<" << ts << ">" << std::endl;
        MKL::Jacobi<T> J;
    }
}

Y_UTEST(eigen_jacobi)
{
    Random::Rand ran;
    doJacobi<float>(ran);
    doJacobi<double>(ran);
    doJacobi<long double>(ran);

}
Y_UDONE()

