//! \file

#ifndef Y_Sort_Heap_Included
#define Y_Sort_Heap_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include <cstring>

namespace Yttrium
{

    struct HeapSort
    {

        //! arr = T[1..n]
        template <
        typename T,
        typename ARRAY,
        typename COMPARE>
        static inline
        void Call(T            arr[],
                  const size_t num,
                  COMPARE     &proc)
        {
            if(num<2) return;

            void *   _[ Y_WORDS_FOR(T) ];
            size_t          il  = (num>>1)+1;
            size_t          ir  = num;
            T       &       __  = *static_cast<T*>(Memory::OutOfReach::Addr(_));
            T       * const a1  = &arr[1];

            while(true)
            {
                if(il>1)
                {
                    Memory::OutOfReach::Copy(_,&arr[--il],sizeof(T));
                }
                else
                {
                    Memory::OutOfReach::Move(_,&arr[ir],a1,sizeof(T));
                    if(--ir == 1)
                    {
                        Memory::OutOfReach::Copy(a1,_,sizeof(T));
                        break;
                    }
                }

                size_t    i   = il;
                size_t    j   = il<<1;
                while (j <= ir)
                {
                    if( (j<ir) && proc(arr[j],arr[j+1]) < 0 )
                            ++j;

                    if( proc(__,arr[j]) < 0 )
                    {
                        Memory::OutOfReach::Copy(&arr[i],&arr[j],sizeof(T));
                        i   = j;
                        j <<= 1;
                    }
                    else
                        break;
                }

                Memory::OutOfReach::Copy(&arr[i],_,sizeof(T));
            }
        }

        //! arr[0..num-1]
        template <typename T, typename COMPARE> static inline
        void Tableau(T arr[], size_t num, COMPARE &proc)
        {
            Call<T,T*,COMPARE>( Memory::OutOfReach::Shift(arr,-1),num,proc);
        }

        
        


    };

}
#endif

