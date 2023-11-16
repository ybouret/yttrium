

//! \file

#ifndef Y_Calculus_Base10_Included
#define Y_Calculus_Base10_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    struct Base10
    {
        static unsigned DigitsForU64(uint64_t u) noexcept;

        template <typename T> static inline
        unsigned DigitsFor(const T u) noexcept
        {
            return DigitsForU64(u);
        }

    };
}

#endif

