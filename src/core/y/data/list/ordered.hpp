
//! \file

#ifndef Y_Data_List_Ordered_Included
#define Y_Data_List_Ordered_Included 1

#include "y/data/list/cxx.hpp"

namespace Yttrium
{

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
    template <typename NODE, typename COMPARATOR>
    class OrderedList : public CxxListOf<NODE>
    {
    public:

        inline explicit OrderedList() noexcept : CxxListOf<NODE>(), compare() {}
        inline virtual ~OrderedList() noexcept {}

        inline void store(NODE *node) noexcept
        {
            ListOps::InsertOrdered(*this,node,compare);
        }

        inline NODE *query() noexcept
        {
            assert(this->size>0);
            return this->popHead();
        }


        COMPARATOR compare;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(OrderedList);
    };

}

#endif
