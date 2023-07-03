
//! \file

#ifndef Y_Type_Signs_Included
#define Y_Type_Signs_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! system wide signs
    //
    //__________________________________________________________________________
    enum SignType
    {
        Negative = -1, //!< alias for negative sign
        __Zero__ =  0, //!< alias for zero sign
        Positive =  1  //!< alias for positive sign
    };


    //______________________________________________________________________
    //
    //
    //! natural comparison to sign
    //
    //______________________________________________________________________
    template <typename LHS, typename RHS> inline
    SignType SignOf(LHS &lhs, RHS &rhs)
    {
        if(lhs<rhs)
            return Negative;
        else
        {
            if(rhs<lhs)
                return Positive;
            else
                return __Zero__;
        }
    }


}


#endif

