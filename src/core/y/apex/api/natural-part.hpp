
//! \file

#ifndef Y_Apex_Natural_Part_Included
#define Y_Apex_Natural_Part_Included 1

#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //! retrieving Natural Part for Natural/Integer
        struct NaturalPart
        {
            static const Natural & Of(const Natural &_) noexcept; //!< return _
            static const Natural & Of(const Integer &_) noexcept; //!< return _.n

        };
    }

}

#endif

