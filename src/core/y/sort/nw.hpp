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
#define Y_NW_FETCH_STATIC(N)                              \
static const Table &          table = Tables::CxxData[N]; \
static const unsigned         swaps = table.swaps;        \
static const unsigned * const start = table.index

    //! helper to fetch data for a given size
#define Y_NW_FETCH_AD_HOC(N)                       \
const Table &          table = Tables::CxxData[N]; \
const unsigned         swaps = table.swaps;        \
const unsigned * const start = table.index

    //! loop CODE on given swaps
#define Y_NW_LOOP(CODE)                  \
/**/    const unsigned *I = start;       \
/**/    for(unsigned i=swaps;i>0;--i)    \
/**/    {                                \
/**/        const unsigned lhs = *(I++); \
/**/        const unsigned rhs = *(I++); \
/**/        CODE                         \
/**/    }

    //! core code execution
#define Y_NW_IMPL_STATIC(CODE)           \
/**/    Y_NW_FETCH_STATIC(N);            \
/**/    Y_NW_LOOP(CODE)

    //! core code execution
#define Y_NW_IMPL_AD_HOC(N,CODE)         \
/**/    assert(n>=Tables::MinSize);      \
/**/    assert(n<=Tables::MaxSize);      \
/**/    Y_NW_FETCH_AD_HOC(N);            \
/**/    Y_NW_LOOP(CODE)

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
                Y_NW_FETCH_STATIC(N);
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
                Y_NW_IMPL_STATIC( if(arr[rhs]<arr[lhs]) Swp(arr[lhs],arr[rhs]); );
            }

            //! make decreasing ARRAY[1..N]
            template <typename ARRAY>
            static inline void Decreasing(ARRAY &arr)
            {
                Y_NW_IMPL_STATIC( if(arr[lhs]<arr[rhs]) Swp(arr[lhs],arr[rhs]); );
            }

            //! make increasing ARRAY[1..N], co-sorting brr[1..N]
            template <typename ARRAY, typename BRRAY>
            static inline void Increasing(ARRAY &arr, BRRAY &brr)
            {
                Y_NW_IMPL_STATIC(
                          if(arr[rhs]<arr[lhs])
                          {
                              Swp(arr[lhs],arr[rhs]);
                              Swp(brr[lhs],brr[rhs]);
                          }
                    );
            }
        };

        //! AdHoc algorithms
        template <typename T>
        struct Make
        {
            //! make increasing arr[1..n]
            template <typename ARRAY>
            static inline void Increasing(ARRAY &arr, const size_t n)
            {
                Y_NW_IMPL_AD_HOC(n,if(arr[rhs]<arr[lhs]) Swp(arr[lhs],arr[rhs]);)
            }

            //! make increasing arr[1..b], co-sorting brr[1..N]
            template <typename ARRAY, typename BRRAY>
            static inline void Increasing(ARRAY &arr, BRRAY &brr, const size_t n)
            {
                Y_NW_IMPL_AD_HOC(n,
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

