//! \file

#ifndef Y_XBitRev_Included
#define Y_XBitRev_Included 1

#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
#include <cstring>

namespace Yttrium
{
    struct XBitRev
    {

        //______________________________________________________________________
        //
        //! bit reversal of data[1..size*2]
        /**
         \param data[1..size*2]
         \param size a power of two
         */
        //______________________________________________________________________
        template <typename T> static inline
        size_t Run_(T data[], const size_t size) noexcept
        {
            static const size_t Required = 2 * sizeof(T);
            void               *tmp[ Y_WORDS_GEQ(Required) ];

            assert(IsPowerOfTwo(size));
            assert(0!=data);
            const size_t n = (size<<1);
            for(size_t j=1,i=1;i<n;i+=2)
            {
                if(j>i)
                {
                    //Swap(data[i],data[j]);
                    //Swap(data[i+1],data[j+1]);
                    T * const lhs = &data[i];
                    T * const rhs = &data[j];
                    memcpy(tmp,lhs,Required);
                    memcpy(lhs,rhs,Required);
                    memcpy(rhs,tmp,Required);
                }
                size_t m=size;
                while (m >= 2 && j > m)
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }
            return n;
        }

        template <typename T> static inline
        size_t Run(T data[], const size_t size) noexcept
        {
            assert(0!=data);
            const size_t n = (size<<1);
            switch(size)
            {
                case 0:
                case 2:
                    return n;

                default:
                    break;
            }
            return Run_(data,size);
        }


    };
}

#endif

