
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
            static Natural Exp(const Natural &base,
                               const Natural &exponent,
                               const Natural &modulus);
        };

    }
}

#endif
