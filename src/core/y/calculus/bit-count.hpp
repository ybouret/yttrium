
//! \file

#ifndef Y_Calculus_Bit_Count_Included
#define Y_Calculus_Bit_Count_Included 1

#include "y/calculus/base2.hpp"

namespace Yttrium
{

    //! Bit counting
    struct BitCount
    {
        //! precomputed table of bits count
        static const uint8_t Table[256];


        //! bits required for a byte
        static inline unsigned For(const uint8_t &b) noexcept {
            return Table[b];
        }

        //! bits required for a word
        static inline unsigned For(const uint16_t &w) noexcept {
            const  unsigned       n = Table[ uint8_t(w>>8) ];
            return (n>0) ? (n+8) : Table[ uint8_t(w) ];
        }

        //! bits required for a dword
        static inline unsigned For(const uint32_t &w) noexcept {
            { const unsigned n = Table[ uint8_t(w>>24) ]; if(n>0) return n+24; }
            { const unsigned n = Table[ uint8_t(w>>16) ]; if(n>0) return n+16; }
            { const unsigned n = Table[ uint8_t(w>>8)  ]; if(n>0) return n+ 8; }
            return Table[ uint8_t(w) ];
        }

        //! bits required for a qword
        static inline unsigned For(const uint64_t &w) noexcept {
            { const unsigned n = Table[ uint8_t(w>>56) ]; if(n>0) return n+56; }
            { const unsigned n = Table[ uint8_t(w>>48) ]; if(n>0) return n+48; }
            { const unsigned n = Table[ uint8_t(w>>40) ]; if(n>0) return n+40; }
            { const unsigned n = Table[ uint8_t(w>>32) ]; if(n>0) return n+32; }
            { const unsigned n = Table[ uint8_t(w>>24) ]; if(n>0) return n+24; }
            { const unsigned n = Table[ uint8_t(w>>16) ]; if(n>0) return n+16; }
            { const unsigned n = Table[ uint8_t(w>>8)  ]; if(n>0) return n+ 8; }
            return Table[ uint8_t(w) ];
        }

    };



    //! bits necessary to write v
    template <typename T>
    inline unsigned BitsFor_(const T v) noexcept
    {
        typedef typename  IntegerFor<T>::UInt::Type U;
        const U  word(v);
        U        mask = Base2<U>::MaxPowerOfTwo;
        unsigned nbit = 8*sizeof(U);
        while(mask)
        {
            if(0!=(word&mask)) return nbit;
            mask >>= 1;
            --nbit;
        }
        return 0;
    }

    //! most_significant_bit of v>=0
    template <typename T>
    inline T MostSignificantBit(const T v) noexcept
    {
        T mask = Base2<T>::MaxPowerOfTwo;
        while(mask)
        {
            if(0!=(v&mask)) return mask;
            mask >>= 1;
        }
        return 0;
    }

}

#endif

