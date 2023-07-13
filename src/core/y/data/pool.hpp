
//! \file

#ifndef Y_Data_Pool_Included
#define Y_Data_Pool_Included 1

#include "y/data/linked.hpp"
#include "y/data/pool/ops.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Linked pool of NODEs
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class PoolOf : public Linked<NODE>
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
        typedef PoolOf<NODE> SelfType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit PoolOf() noexcept : Linked<NODE>()  {}
        virtual ~PoolOf() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline NODE *store(NODE *node)        noexcept { return PoolOps::Store(*this,node); } //!< store a new node
        inline NODE *stash(NODE *node)        noexcept { return PoolOps::Stash(*this,node); } //!< store a new node (slow)
        inline NODE *query()                  noexcept { return PoolOps::Query(*this);      } //!< query first node
        inline NODE *query(const size_t indx) noexcept { return PoolOps::Query(*this,indx); } //!< query node[1..size]

        //! noexcept swap with another pool
        inline void swapWith(PoolOf &other) noexcept
        {
            Swap(head,other.head);
            Swap(Coerce(size),Coerce(other.size));
        }

        //! reverse order
        inline virtual void reverse() noexcept
        {
            SelfType temp;
            while(size>0)
                temp.store( query() );
            swapWith(temp);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(PoolOf);
    };

}

#endif

