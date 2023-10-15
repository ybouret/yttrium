
#include "y/chem/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Canon:: ~Canon() noexcept
        {
        }

        Canon:: Canon(const Conservation &first) :
        Object(), Conservations(), next(0), prev(0)
        {
            (*this) << first;
        }

        bool Canon:: accepts(const Conservation &other) const noexcept
        {
            for(const NodeType *node=head;node;node=node->next)
            {
                const Conservation &mine = **node;
                if(mine.linkedTo(other))
                    return true;
            }
            return false;
        }

        bool Canon:: accepts(const Canon &canon) const noexcept
        {
            for(const NodeType *node=canon.head;node;node=node->next)
            {
                const Conservation &other = **node;
                if(accepts(other))
                    return true;
            }

            return false;
        }


    }

}
