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

        template <>
        struct ExactShift<uint8_t>
        {
            static unsigned Of(const uint8_t) noexcept;
        };

        template <>
        struct ExactShift<uint16_t>
        {
            static unsigned Of(const uint16_t) noexcept;
        };

        template <>
        struct ExactShift<uint32_t>
        {
            static unsigned Of(const uint32_t) noexcept;
        };


        template <>
        struct ExactShift<uint64_t>
        {
            static unsigned Of(const uint64_t) noexcept;
        };
    }

    template <typename T> inline
    unsigned ExactShift(const T x) noexcept
    {
        return Core::ExactShift< typename UnsignedInt<sizeof(T)>::Type > :: Of(x);
    }

}

#endif
