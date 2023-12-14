#include "y/mkl/tao/transmogrify.hpp"
#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"

#include "y/system/rtti.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename TARGET, typename SOURCE>
    static inline void TaoShow()
    {
        typedef Tao::To<TARGET,SOURCE> Trans;
        std::cerr << "TARGET=" << RTTI::Name<TARGET>() << Trans::Text() << "SOURCE=" << RTTI::Name<SOURCE>() << std::endl;

    }

}

Y_UTEST(tao0)
{
    TaoShow<double,double>();

    const float f = 1.2f;
    const XReal<double> x = Tao::To< XReal<double>, float>::Get(f);
    std::cerr << f << " => " << x << std::endl;
    const float         y = Tao::To< float, XReal<double> >::Get(x);
    std::cerr << "y=" << y << std::endl;
}
Y_UDONE()

