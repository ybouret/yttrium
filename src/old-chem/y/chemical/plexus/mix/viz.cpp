#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: viz(OutputStream &fp, const size_t which) const
        {
            assert(which>0);
            if(which>order.size()) return;

            fp("subgraph cluster_%u {\n",my.hallmark);

            for(const SNode *sn=my.species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const String   color = sp.makeColor();
                sp.viz(fp << "\t", color.c_str(), 0);
            }

            const EList &eqs = order[which];
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq    = **en;
                const String       color = eq.makeColor();
                eq.viz(fp,color.c_str(), 0);
            }

            if( 1==which && auth.isValid() )
            {
                for(const Conservation::Law *law=auth->laws->head;law;law=law->next)
                {
                    law->viz(fp);
                }
            }

            fp("}\n");
        }

    }

}
