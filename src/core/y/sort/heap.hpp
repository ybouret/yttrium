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

        template <typename T, typename COMPARE>
        static inline
        void Call(T ra[], const size_t n, COMPARE &proc)
        {
            if(n<2) return;
            void *   _[ Y_WORDS_FOR(T) ];
            size_t   l   =(n >> 1)+1;
            size_t   ir  = n;
            T       &rra = *static_cast<T*>(Memory::OutOfReach::Addr(_));

            while(true)
            {
                if (l>1)
                {
                    memcpy(&rra,&ra[--l],sizeof(T));

                }
                else
                {
                    rra=ra[ir];
                    ra[ir]=ra[1];

                    if(--ir == 1)
                    {
                        ra[1]=rra;
                        break;
                    }
                }

                size_t    i   = l;
                size_t    j   = l+l;

                while (j <= ir)
                {
                    if( (j<ir) && proc(ra[j],ra[j+1])<0 )
                            ++j;


                    if( proc(rra,ra[j]) < 0 )
                    {
                        ra[i]=ra[j];
                        i   = j;
                        j <<= 1;
                    }
                    else
                        break;
                }
                ra[i]=rra;
            }
        }



#if 0
        //! arr[0..num-1] sorted using proc
        template <typename T, typename COMPARE>
        static inline
        void Call(T arr[], const size_t num, COMPARE &proc)
        {
            void *temp[ Y_WORDS_FOR(T) ];
            return Call(arr,num,sizeof(T),temp, Compare<T,COMPARE>, (void *) &proc);
        }
#endif

    private:
        static void Call(void        *base,
                         const size_t size,
                         const size_t itsz,
                         void        *temp,
                         int        (*proc)(const void *,const void *,void*),
                         void        *args) noexcept;

        template <typename T, typename COMPARE> static inline
        int Compare(const void *lhs, const void *rhs, void *args)
        {
            assert(0!=lhs);
            assert(0!=rhs);
            assert(0!=args);
            COMPARE &f = *(COMPARE*)args;
            const T &l = *static_cast<const T *>(lhs);
            const T &r = *static_cast<const T *>(rhs);
            return f(l,r);
        }

    };

}
#endif

