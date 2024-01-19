//! \file

#ifndef Y_Sort_Heap_Included
#define Y_Sort_Heap_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include <cstring>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! HeapSort algorithm
    //
    //__________________________________________________________________________
    struct HeapSort
    {

        //______________________________________________________________________
        //
        //
        //! arr[0..num-1]
        //
        //______________________________________________________________________
        template <typename T, typename COMPARE> static inline
        void Tableau(T arr[], size_t num, COMPARE &proc)
        {
            T *tab =Memory::OutOfReach::Shift(arr,-1);
            Call<T,T*,COMPARE>(tab,num,proc);
        }

        //______________________________________________________________________
        //
        //
        //! arr[0..num-1], brr[0..num-1]
        //
        //______________________________________________________________________
        template <typename T, typename COMPARE, typename U> static inline
        void Tableau(T arr[], size_t num, COMPARE &proc, U brr[])
        {
            T *tab = Memory::OutOfReach::Shift(arr,-1);
            U *tbb = Memory::OutOfReach::Shift(brr,-1);
            Call<T,T*,U,U*,COMPARE>(tab,num,proc,tbb);
        }


        //______________________________________________________________________
        //
        //
        //! arr = T[1..n]
        //
        //______________________________________________________________________
        template <
        typename T,
        typename WRITABLE,
        typename COMPARE>
        static inline
        void Call(WRITABLE    &arr,
                  const size_t num,
                  COMPARE     &proc)
        {
            if(num<2) return;

            void *          wa[ Y_WORDS_FOR(T) ];                                 // temporary
            size_t          il  = (num>>1)+1;                                     // left index
            size_t          ir  = num;                                            // right index
            T       &       ta  = *static_cast<T*>(Memory::OutOfReach::Addr(wa)); // alias
            T       * const a1  = &arr[1];                                        // alias

            while(true) {
                if(il>1)
                {
                    Memory::OutOfReach::Copy(wa,&arr[--il],sizeof(T));
                }
                else
                {
                    Memory::OutOfReach::Move(wa,&arr[ir],a1,sizeof(T));
                    if(--ir == 1)
                    {
                        Memory::OutOfReach::Copy(a1,wa,sizeof(T));
                        break;
                    }
                }

                size_t    i   = il;
                size_t    j   = il<<1;
                while (j <= ir)
                {
                    if( (j<ir) && proc(arr[j],arr[j+1]) < 0 )
                            ++j;

                    if( proc(ta,arr[j]) < 0 )
                    {
                        Memory::OutOfReach::Copy(&arr[i],&arr[j],sizeof(T));
                        i   = j;
                        j <<= 1;
                    }
                    else
                        break;
                }
                Memory::OutOfReach::Copy(&arr[i],wa,sizeof(T));
            }
        }


        //! call on Writable like objects
        template <
        typename WRITABLE,
        typename COMPARE> static inline
        void Call(WRITABLE &arr,
                  COMPARE  &proc)
        {
            Call<typename WRITABLE::Type,WRITABLE,COMPARE>(arr,arr.size(),proc);
        }

        
        //______________________________________________________________________
        //
        //
        //! arr = T[1..n] and brr = U[1..n]
        //
        //______________________________________________________________________
        template <
        typename T,
        typename WRITABLE,
        typename U,
        typename WRITBBLE,
        typename COMPARE>
        static inline
        void Call(WRITABLE    &arr,
                  const size_t num,
                  COMPARE     &proc,
                  WRITBBLE    &brr)
        {
            if(num<2) return;

            void *          wa[ Y_WORDS_FOR(T) ];                                 // temporary
            void *          wb[ Y_WORDS_FOR(U) ];                                 // temporary
            size_t          il  = (num>>1)+1;                                     // left index
            size_t          ir  = num;                                            // right index
            T       &       ta  = *static_cast<T*>(Memory::OutOfReach::Addr(wa)); // alias
            T       * const a1  = &arr[1];                                        // alias
            U       * const b1  = &brr[1];                                        // alias

            while(true) {
                if(il>1)
                {
                    --il;
                    Memory::OutOfReach::Copy(wa,&arr[il],sizeof(T));
                    Memory::OutOfReach::Copy(wb,&brr[il],sizeof(U));
                }
                else
                {
                    Memory::OutOfReach::Move(wa,&arr[ir],a1,sizeof(T));
                    Memory::OutOfReach::Move(wb,&brr[ir],b1,sizeof(U));

                    if(--ir == 1)
                    {
                        Memory::OutOfReach::Copy(a1,wa,sizeof(T));
                        Memory::OutOfReach::Copy(b1,wb,sizeof(U));
                        break;
                    }
                }

                size_t    i   = il;
                size_t    j   = il<<1;
                while (j <= ir)
                {
                    if( (j<ir) && proc(arr[j],arr[j+1]) < 0 )
                        ++j;

                    if( proc(ta,arr[j]) < 0 )
                    {
                        Memory::OutOfReach::Copy(&arr[i],&arr[j],sizeof(T));
                        Memory::OutOfReach::Copy(&brr[i],&brr[j],sizeof(U));
                        i   = j;
                        j <<= 1;
                    }
                    else
                        break;
                }
                Memory::OutOfReach::Copy(&arr[i],wa,sizeof(T));
                Memory::OutOfReach::Copy(&brr[i],wb,sizeof(U));

            }
        }

        //! call on Writable like objects
        template <
        typename WRITABLE,
        typename WRITBBLE,
        typename COMPARE> static inline
        void Call(WRITABLE &arr,
                  COMPARE  &proc,
                  WRITBBLE &brr)
        {
            Call<
            typename WRITABLE::Type,
            WRITABLE,
            typename WRITBBLE::Type,
            WRITBBLE,
            COMPARE>(arr,arr.size(),proc,brr);
        }


       

    };

}
#endif

