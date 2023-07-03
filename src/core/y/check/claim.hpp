


//! \file

#ifndef Y_Check_Claim_Included
#define Y_Check_Claim_Included 1

#include "y/exception.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //! helper for testing conditions
    //__________________________________________________________________________
    struct Claim
    {
        static size_t Width;                                     //!< to align EXPR
        static void   Prolog(const char *expr);                  //!< prolog, aligned
        static void   Epilog(const bool  flag);                  //!< epilog success/failure
        static void   XRaise(const char *expr, const int line);  //!< raise exception
    };

    //__________________________________________________________________________
    //
    //! silent assertion, throw on error
    //__________________________________________________________________________
#define Y_ASSERT(EXPR) do {                               \
/**/ if(!(EXPR)) Yttrium::Claim::XRaise(#EXPR, __LINE__); \
/**/ } while(false)

    //__________________________________________________________________________
    //
    //! verbose assertion, throw on error
    //__________________________________________________________________________
#define Y_CHECK(EXPR) do {                             \
/**/ const char *expr = #EXPR;                         \
/**/ Yttrium::Claim::Prolog(expr);                     \
/**/ const bool  flag = (EXPR);                        \
/**/ Yttrium::Claim::Epilog(flag);                     \
/**/ if(!flag) Yttrium::Claim::XRaise(expr, __LINE__); \
/**/ } while(false)


}

#endif
