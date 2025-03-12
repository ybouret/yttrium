
#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: Cluster(Equilibrium &first) :
        eqs(),
        next(0),
        prev(0)
        {
            eqs << first;
            Coerce(species).collect(first);
        }

        void Cluster:: attach(Equilibrium &eq)
        {
            eqs << eq;
            Coerce(species).collect(eq);
        }


        bool Cluster:: accepts(const Equilibrium &eq) const noexcept
        {
            for(const SNode *sn=species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(eq->search(sp.key())) return true;
            }
            return false;
        }

        bool Cluster:: accepts(const Cluster &cluster) const noexcept
        {
            for(const SNode *sn=species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(cluster.species->has(sp)) return true;
            }
            return false;
        }

    }

}
