
//! \file

#ifndef Y_Apex_Natural_Part_Included
#define Y_Apex_Natural_Part_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        struct NaturalPart
        {
            static const Natural & Of(const Natural &) noexcept;
            static const Natural & Of(const Integer &) noexcept;

        };
    }

}

#endif

