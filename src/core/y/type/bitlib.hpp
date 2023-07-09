
//! \file

#ifndef Y_Type_BitLib_Included
#define Y_Type_BitLib_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{

    struct BitLib
    {
        template <typename OSTREAM, typename T> static inline
        OSTREAM & Display(OSTREAM &os, const T value)
        {
            static const unsigned bits = 8*sizeof(T);
            static const unsigned shl  = bits-1;
            static const T        one(1);
            T mask = one << shl;
            for(unsigned i=bits;i>0;--i,mask>>=1)
            {
                if( 0!= (mask&value) ) os << '1'; else os << '0';
            }
            return os;
        }
        
        template <typename T> static inline
        T LRoll(T value) noexcept
        {
            static const unsigned shl = (8*sizeof(T))-1;
            static const T        one(1);
            static const T        msb  = one << shl;
            return (value << 1) | ( (value&msb) >> shl );
        }
    };

}

#endif

