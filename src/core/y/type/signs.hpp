
//! \file

#ifndef Y_Type_Signs_Included
#define Y_Type_Signs_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    enum SignType
    {
        Negative,
        __Zero__,
        Positive
    };

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

