//! \file

#ifndef Y_Data_List_Included
#define Y_Data_List_Included 1

#include "y/data/linked.hpp"
#include "y/data/list/ops.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Linked list of NODEs
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class ListOf : public Linked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        using   Linked<NODE>::head;
        using   Linked<NODE>::size;
        typedef ListOf<NODE> SelfType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit ListOf() noexcept : Linked<NODE>(), tail(0) {} //!< setup empty
        virtual ~ListOf() noexcept { assert(0==tail); }         //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline NODE *pushTail(NODE *node) noexcept { return ListOps::PushTail(*this,node); } //!< push node at tail
        inline NODE *pushHead(NODE *node) noexcept { return ListOps::PushHead(*this,node); } //!< push node at head
        inline NODE *popTail()            noexcept { return ListOps::PopTail(*this);       } //!< pop head node
        inline NODE *popHead()            noexcept { return ListOps::PopHead(*this);       } //!< pop tail node
        inline NODE *pop(NODE *node)      noexcept { return ListOps::Pop(*this,node);      } //!< pop any node

        //! insert node after mine
        inline NODE *insertAfter(NODE *mine, NODE *node) noexcept
        {
            return ListOps:: InsertAfter(*this,mine,node);
        }

        //! insert node by increasing address
        inline NODE *insertByIncreasingAddress(NODE *node) noexcept
        {
            return ListOps::InsertByIncreasingAddress(*this,node);
        }

        //! noexcept swap with another list
        inline void swapWith(ListOf &other) noexcept
        {
            Swap(tail,other.tail);
            Swap(head,other.head);
            Swap(Coerce(size),Coerce(other.size));
        }

        //! reverse list order
        inline virtual void reverse() noexcept
        {
            SelfType temp;
            while(size>0)
                temp.pushHead( popHead() );
            swapWith(temp);
        }

        //! move node to front
        NODE *moveToFront(NODE *node) noexcept
        {
            return ListOps:: MoveToFront(*this,node);
        }

        //! merge content at tail
        inline void mergeTail(ListOf &other) noexcept
        {
            ListOps:: MergeTail(*this,other);
        }

        //! merge content at tail
        inline void mergeHead(ListOf &other) noexcept
        {
            ListOps:: MergeTail(other,*this);
            swapWith(other);
        }


        //______________________________________________________________________
        //
        //
        // Member
        //
        //______________________________________________________________________
        NODE *tail; //!< tail node

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ListOf);
    };

}

#endif

