
//! \file

#ifndef Y_Sort_Merge_Included
#define Y_Sort_Merge_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    struct MergeSort
    {

        template <typename LIST, typename COMPARE_NODES> static inline
        void Call(LIST &L, COMPARE_NODES &compareNodes)
        {
            // internal definitions
            typedef typename LIST::NodeType NodeType;
            typedef ListOf<NodeType>        ListType;

            // recursive call
            if(L.size>=2)
            {
                ListType lhs,rhs;
                ListOps::Divide(lhs,rhs,L);
                Call(lhs,compareNodes);
                Call(rhs,compareNodes);
                ListOps::Fusion(L,lhs,rhs,compareNodes);
            }

        }



    };

}

#endif
