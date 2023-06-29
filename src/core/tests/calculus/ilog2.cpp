
#include "y/calculus/ilog2.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

Y_UTEST(calculus_ilog2)
{
    std::cerr << iLog2<1>::Value   << std::endl;
    std::cerr << iLog2<2>::Value   << std::endl;
    std::cerr << iLog2<4>::Value   << std::endl;
    std::cerr << iLog2<8>::Value   << std::endl;
    std::cerr << iLog2<16>::Value  << std::endl;
    std::cerr << iLog2<32>::Value  << std::endl;
    std::cerr << iLog2<64>::Value  << std::endl;
    std::cerr << iLog2<128>::Value << std::endl;
    std::cerr << iLog2<256>::Value << std::endl;
    std::cerr << iLog2<512>::Value << std::endl;
}
Y_UDONE()
