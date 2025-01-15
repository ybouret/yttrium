

//! \file

#ifndef Y_Data_GC_Cxx_Pool_Included
#define Y_Data_GC_Cxx_Pool_Included 1

#include "y/data/gc/cxx-list.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/data/rework.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! GC for CxxPool
    //
    //
    //__________________________________________________________________________
    struct CxxPoolGC
    {
        //! reduce size after memory ordering
        template <typename NODE> static inline
        void Cycle(CxxPoolOf<NODE> &P, const size_t cycles) noexcept {
            CxxListOf<NODE> L;
            Rework::PoolToList(L,P);
            CxxListGC::Cycle(L,cycles);
            Rework::ListToPool(P,L);
        }
    };

}

#endif

