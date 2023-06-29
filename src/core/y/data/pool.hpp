
//! \file

#ifndef Y_Data_Pool_Included
#define Y_Data_Pool_Included 1

#include "y/data/linked.hpp"
#include "y/data/pool/ops.hpp"

namespace Yttrium
{

    template <typename NODE>
    class PoolOf : public Linked<NODE>
    {
    public:
        using   Linked<NODE>::head;
        typedef PoolOf<NODE> SelfType;

        explicit PoolOf() noexcept : Linked<NODE>()  {}
        virtual ~PoolOf() noexcept {}

        inline NODE *store(NODE *node) noexcept { return PoolOps::Store(*this,node); }
        inline NODE *query()           noexcept { return PoolOps::Query(*this);      }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(PoolOf);
    };

}

#endif

