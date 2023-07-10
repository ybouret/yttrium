
//! \file

#ifndef Y_Comparison_Included
#define Y_Comparison_Included 1

#include "y/type/signs.hpp"

namespace Yttrium
{

    struct Comparison
    {
        template <typename T> static inline
        int Increasing(const T &lhs, const T &rhs)
        {
            return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0);
        }
    };

}

#endif

