//! \file

#ifndef Y_NetworkSort_Included
#define Y_NetworkSort_Included 1

#include "y/sort/nw/tables.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iostream>
#include <iomanip>

namespace Yttrium
{

    //! helper to fetch data for a given size
#define Y_NW_FETCH(N)                                     \
static const Table &          table = Tables::CxxData[N]; \
static const unsigned         swaps = table.swaps;        \
static const unsigned * const start = table.index

    //! core code executiong
#define Y_NW_IMPL(CODE)                  \
/**/    Y_NW_FETCH(N);                   \
/**/    const unsigned *I = start;       \
/**/    for(unsigned i=swaps;i>0;--i)    \
/**/    {                                \
/**/        const unsigned lhs = *(I++); \
/**/        const unsigned rhs = *(I++); \
/**/        CODE                         \
/**/    }

    namespace NetworkSort
    {
        //______________________________________________________________________
        //
        //
        //
        //! Common procedures
        //
        //
        //______________________________________________________________________
        struct Common
        {
            //! display info for a given table
            template <size_t N> static inline
            void Display()
            {
                assert(N>=Tables::MinSize);
                assert(N<=Tables::MaxSize);
                Y_NW_FETCH(N);
                std::cerr << "#NetworkSort" << std::setw(2) << N << " : swaps=" << std::setw(4) << swaps << std::endl;
                const unsigned *I = start;
                for(unsigned i=swaps;i>0;--i)
                {
                    const unsigned lhs = *(I++);
                    const unsigned rhs = *(I++);
                    std::cerr << " [" << lhs << "," << rhs << "]";
                }
                std::cerr  << std::endl;
            }
        };

        //______________________________________________________________________
        //
        //
        //
        //! local swap
        //
        //
        //______________________________________________________________________
        template <typename T> static inline
        void Swp(T &lhs, T &rhs) noexcept
        {
            Memory::OutOfReach::Swap(lhs,rhs);
        }

        //______________________________________________________________________
        //
        //
        //
        //! Build Algorithm for a given size
        //
        //
        //______________________________________________________________________
        template <size_t N, typename T>
        struct Algo
        {
            //! make increasing ARRAY[1..N]
            template <typename ARRAY>
            static inline void Increasing(ARRAY &arr)
            {
                Y_NW_IMPL( if(arr[rhs]<arr[lhs]) Swp(arr[lhs],arr[rhs]); );
            }

            //! make decreasing ARRAY[1..N]
            template <typename ARRAY>
            static inline void Decreasing(ARRAY &arr)
            {
                Y_NW_IMPL( if(arr[lhs]<arr[rhs]) Swp(arr[lhs],arr[rhs]); );
            }

            //! make increasing ARRAY[1..N], co-sorting brr[1..N]
            template <typename ARRAY, typename BRRAY>
            static inline void Increasing(ARRAY &arr, BRRAY &brr)
            {
                Y_NW_IMPL(
                          if(arr[rhs]<arr[lhs])
                          {
                              Swp(arr[lhs],arr[rhs]);
                              Swp(brr[lhs],brr[rhs]);
                          }
                    );
            }

        };

    }
}

#endif

