//! \file

#ifndef Y_Sort_Heap_Included
#define Y_Sort_Heap_Included 1

#include "y/calculus/align.hpp"

namespace Yttrium
{

    struct HeapSort
    {

        //! arr[0..num-1] sorted using proc
        template <typename T, typename COMPARE>
        static inline
        void Call(T arr[], const size_t num, COMPARE &proc)
        {
            void *temp[ Y_WORDS_FOR(T) ];
            return Call(arr,num,sizeof(T),temp, Compare<T,COMPARE>, (void *) &proc);
        }

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

