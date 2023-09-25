
#include "y/calculus/base2.hpp"
#include "y/calculus/xlog2.hpp"

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
    void TestXLog2(const uint64_t *xtab)
    {
        static const unsigned SZ = sizeof(T);
        static const unsigned NB = SZ * 8;
        T xloc[NB] = { 0 };
        for(unsigned i=0;i<NB;++i) xloc[i] = static_cast<T>(xtab[i]);
        if(false)
        {
            std::cerr << std::endl;
            for(size_t i=0;i<NB;++i)
                std::cerr << " 0x" << Hexadecimal(xloc[i],Hexadecimal::Compact) << ",";
            std::cerr << std::endl;
        }

        for(unsigned i=0;i<NB;++i)
        {
            volatile unsigned p = ExactLog2(xloc[i]);
            volatile unsigned q = Base2<T>::Log(xloc[i]);
            Y_ASSERT(p==q);
        }

        std::cerr << "uint" << std::setw(3) << NB;
        Timing tmx;

        Y_Timing(tmx, 
                 for(unsigned i=0;i<NB;++i)
                 { volatile unsigned p = ExactLog2(xloc[i]); (void)p; },
                 0.5);
        (std::cerr << " | xlog2: " << HumanReadable(tmx.speed())).flush();

        Y_Timing(tmx,
                 for(unsigned i=0;i<NB;++i)
                 { volatile unsigned p = Base2<T>::Log(xloc[i]); (void)p; },
                 0.5);
        std::cerr << " | blog2: " << HumanReadable(tmx.speed()) << std::endl;

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


    TestXLog2<uint8_t>(  Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint16_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint32_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );
    TestXLog2<uint64_t>( Memory::OutOfReach::Cast<uint64_t>(xtab) );




}
Y_UDONE()

