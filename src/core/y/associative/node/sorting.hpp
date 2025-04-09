//! \file

#ifndef Y_Associative_Node_Sorting_Included
#define Y_Associative_Node_Sorting_Included 1

#include "y/sort/merge.hpp"

namespace Yttrium
{

    template <typename NODE>
    class AssociativeNodeSorting
    {
    protected:
        inline explicit AssociativeNodeSorting() noexcept {}


    public:
        inline virtual ~AssociativeNodeSorting() noexcept {}

        //! sort list of nodes by value
        template <typename PROC> inline
        void sortByValue(PROC &proc)
        {
            CompareByValue<PROC> comparison = { proc };
            MergeSort::Call(getList(),comparison);
        }

        //! sort node by content
        template <typename PROC> inline
        void sortByKey(PROC &proc)
        {
            CompareByKey<PROC> comparison = { proc };
            MergeSort::Call(getList(),comparison);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(AssociativeNodeSorting);

        virtual ListOf<NODE> & getList() noexcept = 0;


        template <typename PROC>
        struct CompareByValue
        {
            PROC &proc;
            inline SignType operator()(const NODE * const lhs, const NODE * const rhs) const
            {
                return proc(**lhs,**rhs);
            }
        };

        template <typename PROC>
        struct CompareByKey
        {
            PROC &proc;
            inline SignType operator()(const NODE * const lhs, const NODE * const rhs) const
            {
                return proc(lhs->key,rhs->key);
            }
        };
    };

}


#endif

