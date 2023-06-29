//! \file

#ifndef Y_Data_List_Included
#define Y_Data_List_Included 1

#include "y/data/linked.hpp"
#include "y/data/list/ops.hpp"

namespace Yttrium
{

    template <typename NODE>
    class ListOf : public Linked<NODE>
    {
    public:
        using   Linked<NODE>::head;
        typedef ListOf<NODE> SelfType;

        explicit ListOf() noexcept : Linked<NODE>(), tail(0) {}
        virtual ~ListOf() noexcept { assert(0==tail); }


        inline NODE *pushTail(NODE *node) noexcept { return ListOps::PushTail(*this,node); }
        inline NODE *pushHead(NODE *node) noexcept { return ListOps::PushHead(*this,node); }
        inline NODE *popTail() noexcept { return ListOps::PopTail(*this); }
        inline NODE *popHead() noexcept { return ListOps::PopHead(*this); }

        NODE *tail;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ListOf);
    };

}

#endif

