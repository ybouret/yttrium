
#include "y/chem/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Canon:: ~Canon() noexcept
        {
        }

        Canon:: Canon(const Conservation &first) :
        Object(), Conservations(), repo(), next(0), prev(0)
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

        void Canon:: compile()
        {
            // record repository
            SpRepo &here = Coerce(repo);
            for(const NodeType *node=head;node;node=node->next)
            {
                const Conservation &cons = **node;
                for(const Actor *a=cons->head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    if(here.has(sp)) continue;
                    here << sp;
                }
                
            }

            // sort by increasing top-level index
            SortIncreasing(here);

#if 0
            // make aux-level indices
            size_t      indx=1;
            for(SpNode *node=here.head;node;node=node->next,++indx)
            {
                const Species &sp = **node;
                Coerce(sp.indx[AuxLevel]) = indx;
            }
#endif
        }

        void Canon:: viz(OutputStream &fp, size_t &indx) const
        {
            for(const NodeType *node=head;node;node=node->next,++indx)
            {
                const Conservation &cons = **node;
                cons.viz(fp,indx);
            }
        }

    }

}
