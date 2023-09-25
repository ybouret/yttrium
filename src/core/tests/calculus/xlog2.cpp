
#include "y/calculus/bit-count.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/random/shuffle.hpp"
#include "y/text/hexadecimal.hpp"
#include <cstdio>

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline
    unsigned ExactLog2(const T u)
    {
        Y_ASSERT( IsPowerOfTwo(u) );
        return 0;
    }

    template <typename T>
    static inline
    void TestXLog2(const uint64_t *xtab)
    {
        static const unsigned SZ = sizeof(T);
        static const unsigned NB = SZ * 8;
        T xloc[NB] = { 0 };
        for(unsigned i=0;i<NB;++i) xloc[i] = static_cast<T>(xtab[i]);
        for(size_t i=0;i<NB;++i)
            std::cerr << " 0x" << Hexadecimal(xloc[i],Hexadecimal::Compact);
        std::cerr << std::endl;
        
    }

}

Y_UTEST(calculus_xlog2)
{

    uint64_t xtab[64] = { 0 };
    for(unsigned i=0;i<64;++i)
    {
        const uint64_t one=1;
        xtab[i] = one << i;
    }


    TestXLog2<uint8_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint16_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint32_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint64_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );




}
Y_UDONE()

