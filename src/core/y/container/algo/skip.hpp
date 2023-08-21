

//! \file

#ifndef Y_Container_Algo_Skip_Included
#define Y_Container_Algo_Skip_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Algo
    {
        //______________________________________________________________________
        //
        //
        //! Skip first bad items
        //
        //______________________________________________________________________
        template <typename SEQUENCE, typename IS_BAD>
        inline void Skip(SEQUENCE &seq, IS_BAD &is_bad)
        {
            while( seq.size() && is_bad(seq.head())) seq.popHead();
        }
    }

}
#endif

