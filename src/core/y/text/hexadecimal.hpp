//! \file

#ifndef Y_Text_Hexadecimal_Included
#define Y_Text_Hexadecimal_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Hexadecimal utilities
    //
    //
    //__________________________________________________________________________
    struct Hexadecimal
    {
        static const char Text[256][4];                   //!< "00".."ff"
        static int        ToDecimal(const char) noexcept; //!< -1 if invalid
    };
}

#endif

