//! \file

#ifndef Y_Type_Destruct_Included
#define Y_Type_Destruct_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //______________________________________________________________________
    //
    //
    //! call item's destructor
    //
    //______________________________________________________________________
    template <typename T>
    inline void Destruct(T *item) noexcept
    {
        assert(0!=item);
        item->~T();
    }

    //______________________________________________________________________
    //
    //
    //! return destructed item address
    //
    //______________________________________________________________________
    template <typename T>
    inline T *Destructed(T *item) noexcept
    {
        Destruct(item);
        return item;
    }

}

#endif

