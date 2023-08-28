//! \file

#ifndef Y_NetworkSort_Included
#define Y_NetworkSort_Included 1

#include "y/sort/nw/tables.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iostream>
#include <iomanip>

namespace Yttrium
{

#define Y_NW_FETCH(N) \
static const Table &          table = Tables::CxxData[N]; \
static const unsigned         swaps = table.swaps;        \
static const unsigned * const start = table.index

#define Y_NW_IMPL(CODE) \
/**/    Y_NW_FETCH(N);\
/**/    const unsigned *I = start;\
/**/    for(unsigned i=swaps;i>0;--i)\
/**/    {\
/**/        const unsigned lhs = *(I++);\
/**/        const unsigned rhs = *(I++);\
/**/        CODE\
/**/    }

    namespace NetworkSort
    {
        struct Common
        {
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

        template <typename T> static inline
        void Swp(T &lhs, T &rhs) noexcept
        {
            Memory::OutOfReach::Swap(lhs,rhs);
        }

        template <size_t N, typename T>
        struct Algo
        {
            template <typename ARRAY>
            static inline void Increasing(ARRAY &arr)
            {
                Y_NW_IMPL( if(arr[rhs]<arr[lhs]) Swp(arr[lhs],arr[rhs]); );
            }

            template <typename ARRAY>
            static inline void Decreasing(ARRAY &arr)
            {
                Y_NW_IMPL( if(arr[lhs]<arr[rhs]) Swp(arr[lhs],arr[rhs]); );
            }

        };

    }
}

#endif

