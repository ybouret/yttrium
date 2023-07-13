
//! \file

#ifndef Y_Sort_Merge_Included
#define Y_Sort_Merge_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    struct MergeSort
    {

        template <typename LIST> static inline
        void Call(LIST &L)
        {
            if(L.size<2) return;

            typedef typename LIST::NodeType NodeType;
            typedef ListOf<NodeType>        ListType;

            ListType stack[8];
            ListOps::Divide(stack[0],stack[1],L);

        }


    };

}

#endif
