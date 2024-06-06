
//! \file

#ifndef Y_Kemp_Modular_Included
#define Y_Kemp_Modular_Included 1

#include "y/kemp/natural.hpp"

namespace Yttrium
{
    namespace Kemp
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

