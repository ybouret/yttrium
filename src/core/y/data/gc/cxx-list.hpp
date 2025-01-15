
//! \file

#ifndef Y_Data_GC_Cxx_List_Included
#define Y_Data_GC_Cxx_List_Included 1

#include "y/data/list/cxx.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! GC for CxxList
    //
    //
    //__________________________________________________________________________
    struct CxxListGC
    {

        //! compute new max size, always keep one element if possible
        template <typename NODE>
        static inline size_t NewMaxSize(const Linked<NODE> &L) noexcept {
            const size_t guess = L.size>>1;
            return guess <= 1 ? 1 : guess;
        }

        //! reduce size after memory ordering
        template <typename NODE> static inline
        void Cycle(CxxListOf<NODE> &L, size_t cycles) noexcept {
            MergeSort::Call(L,ListOps::IncreasingAddresses<NODE>);
            while(cycles-- > 0 ) {
                if( CutOrdered(L) <= 1) return;
            }
        }

        //! delete about half the nodes with higher addresses
        template <typename NODE>
        static inline size_t CutOrdered(CxxListOf<NODE> &L) noexcept {
            assert(ListOps::CheckIncreasingAddresses(L));
            const size_t newMaxSize = NewMaxSize(L);
            while(L.size>newMaxSize) delete L.popTail();
            return L.size;
        }
    };
}

#endif

