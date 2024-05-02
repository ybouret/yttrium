//! \file

#ifndef Y_Color_Scalar_Conv_Included
#define Y_Color_Scalar_Conv_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Color
    {

        template <typename TARGET, typename SOURCE>
        struct ScalarConv
        {
            static TARGET From(const SOURCE source) noexcept;
        };

        template <typename TARGET>
        struct ScalarConv<TARGET,TARGET>
        {
            static inline TARGET From(const TARGET value) noexcept { return value; }
        };

        
    }

}

#endif
