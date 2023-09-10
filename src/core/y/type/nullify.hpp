
//! \file

#ifndef Y_Type_Nullify_Included
#define Y_Type_Nullify_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //______________________________________________________________________
    //
    //
    //! call item's delete
    //
    //______________________________________________________________________
    template <typename T>
    inline void Nullify(T * &item) noexcept
    {
        delete item;
        item=0;
    }



}

#endif

