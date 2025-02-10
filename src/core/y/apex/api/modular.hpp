

//! \file

#ifndef Y_Apex_Modular_Included
#define Y_Apex_Modular_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Modular Arithmetic
        //
        //
        //______________________________________________________________________
        struct Modular
        {
            //! b^e[n]
            static Natural Exp(const Natural &b,
                               const Natural &e,
                               const Natural &n);

            //! a^(-1)[n]
            static Natural Inv(const Natural &a,
                               const Natural &n);
        };

    }
}

#endif

