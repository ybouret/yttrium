

//! \file

#ifndef Y_Container_Algo_Crop_Included
#define Y_Container_Algo_Crop_Included 1

#include "y/container/algo/trim.hpp"
#include "y/container/algo/skip.hpp"

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
        inline SEQUENCE &Crop(SEQUENCE &seq, IS_BAD &bad)
        {
            return Skip(Trim(seq,bad),bad);
        }
    }

}
#endif

