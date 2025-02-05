//! \file

#ifndef Y_Exact_Shift_Included
#define Y_Exact_Shift_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Core
    {
        template <typename T>
        struct ExactShift;

        //----------------------------------------------------------------------
        //
        //! exact shift for 8 bits
        //
        //----------------------------------------------------------------------
        template <>
        struct ExactShift<uint8_t>
        {
            static unsigned Of(const uint8_t) noexcept; //!< 0..7, 8=error
        };

        //----------------------------------------------------------------------
        //
        //! exact shift for 16 bits
        //
        //----------------------------------------------------------------------
        template <>
        struct ExactShift<uint16_t>
        {
            static unsigned Of(const uint16_t) noexcept; //!< 0..15, 16=error
        };

        //----------------------------------------------------------------------
        //
        //! exact shift for 32 bits
        //
        //----------------------------------------------------------------------
        template <>
        struct ExactShift<uint32_t>
        {
            static unsigned Of(const uint32_t) noexcept; //!< 0..31, 32=error
        };

        //----------------------------------------------------------------------
        //
        //! exact shift for 64 bits
        //
        //----------------------------------------------------------------------
        template <>
        struct ExactShift<uint64_t>
        {
            static unsigned Of(const uint64_t) noexcept; //!< 0..63, 64=error
        };
    }


    //--------------------------------------------------------------------------
    //
    //
    //! exact shift for unsigned integral type
    //
    //
    //--------------------------------------------------------------------------
    template <typename T> inline
    unsigned ExactShift(const T x) noexcept
    {
        return Core::ExactShift< typename UnsignedInt<sizeof(T)>::Type > :: Of(x);
    }

}

#endif
