//! \file

#ifndef Y_Container_Algo_Reverse_Included
#define Y_Container_Algo_Reverse_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Algo
    {

        //______________________________________________________________________
        //
        //
        //! reverse elements using iterators algebra
        //
        //______________________________________________________________________
        template <typename ITERATOR>
        inline void Reverse(ITERATOR     head,
                            const size_t size) noexcept
        {
            if(size<=1) return;
            ITERATOR tail = head+size-1;
            for(size_t i=size>>1;i>0;--i,++head,--tail)
            {
                Memory::OutOfReach::Swap(*head,*tail);
            }
        }
    }
}

#endif

