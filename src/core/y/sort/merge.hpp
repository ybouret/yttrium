
//! \file

#ifndef Y_Sort_Merge_Included
#define Y_Sort_Merge_Included 1

#include "y/data/list.hpp"
#include "y/comparison.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! MergeSort algorithms
    //
    //
    //__________________________________________________________________________
    struct MergeSort
    {
        //______________________________________________________________________
        //
        //
        //! sort a list
        /**
         \param L            list of nodes
         \param compareNodes is SignType compareNodes(const NODE *, const NODE *)
         */
        //
        //______________________________________________________________________
        template <typename NODE, typename COMPARE_NODES> static inline
        void Call(ListOf<NODE> &L, COMPARE_NODES &compareNodes)
        {
            // internal definitions
            typedef ListOf<NODE>        ListType;

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

        //______________________________________________________________________
        //
        //
        //! sort nodes by increasing address
        //
        //______________________________________________________________________
        template <typename NODE> static inline
        void ByIncreasingAddress(ListOf<NODE> &L)
        {
            return Call(L,Comparison::CxxIncreasing<NODE*>);
        }


    };

}

#endif
