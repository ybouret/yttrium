#include "y/random/shuffle.hpp"
//#include <iostream>

namespace Yttrium
{
    namespace Random
    {

        

#if 0
        void Shuffle:: Tableau(void        *base,
                               const size_t size,
                               const size_t itemSize,
                               Bits         &ran) noexcept
        {
            assert(Good(base,size));
            assert(itemSize>0);
            uint8_t   *p = static_cast<uint8_t *>(base);
            uint8_t   *q = p;
            p += itemSize*size;
            for(size_t i=size-1;i>0;--i)
            {
                p -= itemSize; assert(p>=base);
                const size_t j = ran.leq(i);
                if(j<i)
                {
                    Exch(p,q+j*itemSize,itemSize);
                }
            }
        }

#endif

    }

}

