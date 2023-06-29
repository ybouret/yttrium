#include "y/calculus/align.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

Y_UTEST(calculus_align)
{
    for(size_t i=0;i<=10;++i)
    {
        std::cerr << i << " -> " << Y_ALIGN_ON(2,i) << " -> " << Y_ALIGN_ON(8,i) << std::endl;
    }
}
Y_UDONE()
