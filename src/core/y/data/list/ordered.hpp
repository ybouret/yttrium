
//! \file

#ifndef Y_Data_List_Ordered_Included
#define Y_Data_List_Ordered_Included 1

#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    enum OrderedListQuery
    {
        OrderedListQueryHead,
        OrderedListQueryTail
    };

    //__________________________________________________________________________
    //
    //
    //
    //! Ordered List with Pool API
    /**
     - NODE       is a doubly linked node
     - COMPARATOR must provide a SignType operator
     */
    //__________________________________________________________________________
    template <
    typename         NODE,
    typename         COMPARATOR,
    OrderedListQuery QUERY>
    class OrderedList : public CxxListOf<NODE>
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit OrderedList() noexcept : CxxListOf<NODE>(), compare() {} //!< setup
        inline virtual ~OrderedList() noexcept {}                                //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
      
        //! store using ordered insertion
        inline void store(NODE *node) noexcept
        {
            ListOps::InsertOrdered(*this,node,compare);
        }

        //! query
        inline NODE *query() noexcept
        {
            static const Int2Type<QUERY> choice = {};
            return query(choice);
        }


        COMPARATOR compare;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(OrderedList);
        inline NODE *query(const Int2Type<OrderedListQueryHead> & ) noexcept
        {
            assert(this->size>0);
            return this->popHead();
        }

        inline NODE *query(const Int2Type<OrderedListQueryTail> & ) noexcept
        {
            assert(this->size>0);
            return this->popTail();
        }
    };

}

#endif
