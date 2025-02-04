#include "y/calculus/exact-shift.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(calculus_exact_shift)
{

    if(false)
    {
        for(unsigned i=0;i<64;++i)
        {
            std::cerr << "\tY_Exact_Shift(" << i << ");" << std::endl;
        }
    }

    std::cerr << "Testing 8-bits" << std::endl;
    for(unsigned shift=0;shift<8;++shift)
    {
        static const uint8_t _1 = 1;
        const uint8_t        x  = _1 << shift;
        Y_ASSERT( shift == ExactShift(x) );
    }

    std::cerr << "Testing 16-bits" << std::endl;
    for(unsigned shift=0;shift<16;++shift)
    {
        static const uint16_t _1 = 1;
        const uint16_t        x  = _1 << shift;
        Y_ASSERT( shift == ExactShift(x) );
    }

    std::cerr << "Testing 32-bits" << std::endl;
    for(unsigned shift=0;shift<32;++shift)
    {
        static const uint32_t _1 = 1;
        const uint32_t        x  = _1 << shift;
        Y_ASSERT( shift == ExactShift(x) );
    }

    std::cerr << "Testing 64-bits" << std::endl;
    for(unsigned shift=0;shift<32;++shift)
    {
        static const uint64_t _1 = 1;
        const uint64_t        x  = _1 << shift;
        Y_ASSERT( shift == ExactShift(x) );
    }


}
Y_UDONE()

