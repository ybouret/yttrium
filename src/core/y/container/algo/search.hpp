//! \file

#ifndef Y_Container_Algo_Search_Included
#define Y_Container_Algo_Search_Included 1

#include "y/sequence/interface.hpp"

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
        inline SEQUENCE &Search(SEQUENCE &seq, IS_BAD &is_bad)
        {
            while( seq.size() && is_bad(seq.head())) seq.popHead();
            return seq;
        }
    }

}
#endif
