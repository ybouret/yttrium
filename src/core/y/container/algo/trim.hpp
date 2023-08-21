
//! \file

#ifndef Y_Container_Algo_Trim_Included
#define Y_Container_Algo_Trim_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Algo
    {
        //______________________________________________________________________
        //
        //
        //! Trim last bad items
        //
        //______________________________________________________________________
        template <typename SEQUENCE, typename IS_BAD>
        inline void Trim(SEQUENCE &seq, IS_BAD &is_bad)
        {
            while( seq.size() && is_bad(seq.tail())) seq.popTail();
        }
    }

}
#endif

