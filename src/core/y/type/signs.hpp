
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

    
    template <typename T> inline
    SignType SignOf(T &value)
    {
        return value < 0 ? Negative : (0<value? Positive : __Zero__);
    }

    //__________________________________________________________________________
    //
    //
    //! natural comparison to sign
    //
    //__________________________________________________________________________
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

