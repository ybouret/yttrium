
//! \file

#ifndef Y_Calculus_Byte_Count_Included
#define Y_Calculus_Byte_Count_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{

    //! Bit counting
    struct ByteCount
    {
        //! bytes required for a byte
        static inline unsigned For(const uint8_t &b) noexcept {
           return UnsignedInt<1>::BytesFor(b);
        }

        //! bytes required for a word
        static inline unsigned For(const uint16_t &w) noexcept {
            return UnsignedInt<2>::BytesFor(w);
        }

        //! bytes required for a word
        static inline unsigned For(const uint32_t &dw) noexcept {
            return UnsignedInt<4>::BytesFor(dw);
        }


        //! bytes required for a word
        static inline unsigned For(const uint64_t &qw) noexcept {
            return UnsignedInt<8>::BytesFor(qw);
        }
    };

}

#endif
