//! \file

#ifndef Y_Type_Pick_Included
#define Y_Type_Pick_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //! default type selector.
    //__________________________________________________________________________
    template <const bool flag, typename T, typename U> struct Pick;

    //__________________________________________________________________________
    //
    //! type selector when true => T
    //__________________________________________________________________________
    template <typename T, typename U>
    struct Pick<true,T,U>
    {
        typedef T Type; //!< flag is true
    };

    //__________________________________________________________________________
    //
    //! type selector when false => U
    //__________________________________________________________________________
    template <typename T, typename U>
    struct Pick<false,T,U>
    {
        typedef U Type; //!< flag is false
    };

}

#endif
