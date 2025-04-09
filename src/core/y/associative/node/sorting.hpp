//! \file

#ifndef Y_Associative_Node_Sorting_Included
#define Y_Associative_Node_Sorting_Included 1

#include "y/sort/merge.hpp"
#include "y/random/shuffle.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Base class to allowe sorting of internal lists
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class AssociativeNodeSorting
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit AssociativeNodeSorting() noexcept {} //!< setup
        inline virtual ~AssociativeNodeSorting() noexcept {} //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

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

        //! shuffle internal list
        inline void shuffle(Random::Bits &ran)
        {
            Random::Shuffle::List( getList(), ran);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(AssociativeNodeSorting);

        //! get internal compatible list
        virtual ListOf<NODE> & getList() noexcept = 0;

        //! wrapper to forward call on nodes value
        template <typename PROC>
        struct CompareByValue
        {
            PROC &proc;
            inline SignType operator()(const NODE * const lhs, const NODE * const rhs) const
            {
                return proc(**lhs,**rhs);
            }
        };

        //! wrapper to forwad code on nodes key
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

