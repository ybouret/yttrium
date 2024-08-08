//! \file

#ifndef Y_Text_Boolean_Included
#define Y_Text_Boolean_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{

    //! human readable boolean
    struct BooleanTo
    {
        static const char *Text(const bool flag) noexcept; //!< True|False
        static const char *text(const bool flag) noexcept; //!< true|false
    };

}

#endif

