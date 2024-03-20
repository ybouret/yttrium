
//! \file

#ifndef Y_Apex_Modular_Included
#define Y_Apex_Modular_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {

        struct Modular
        {
            static Natural Exp(const Natural &b,
                               const Natural &e,
                               const Natural &n);

            static Natural Inv(const Natural &a,
                               const Natural &n);
        };

    }
}

#endif

