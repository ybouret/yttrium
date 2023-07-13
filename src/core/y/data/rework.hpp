//! \file

#ifndef Y_Data_Rework_Included
#define Y_Data_Rework_Included 1

#include "y/data/pool.hpp"
#include "y/data/list.hpp"

namespace Yttrium
{

    struct Rework
    {
        template <typename NODE> static inline
        void PoolToList(ListOf<NODE> &)
        {

        };
    };

}

#endif

