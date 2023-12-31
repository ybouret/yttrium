
//! \file

#ifndef Y_Type_Addition_Included
#define Y_Type_Addition_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! helper for careful addition
    //
    //__________________________________________________________________________
    struct Addition
    {
        //______________________________________________________________________
        //
        //! throw exception with optional context
        //______________________________________________________________________
        static void RaiseOverflow(const char *ctx);


        //______________________________________________________________________
        //
        //! return lhs+rhs with overflow control
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        LHS Of(const LHS lhs,
               const RHS rhs,
               const char *ctx)
        {
            assert(rhs>=0);
            typedef IntegerFor<LHS> Info;
            static const LHS        lhsMax = Info::Maximum;
            const        LHS        rhsMax = lhsMax-lhs;
            if(rhs>rhsMax) RaiseOverflow(ctx);
            return lhs+rhs;
        }
    };

}

#endif

