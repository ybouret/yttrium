
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterType:: ClusterType() noexcept : Fragment(), equilibria(), species()
        {
        }

        ClusterType:: ~ClusterType() noexcept
        {
        }

        void ClusterType:: link(Equilibrium &eq)
        {
            assert(!equilibria->has(eq));
            {
                SubEList esave(equilibria);
                SubSList ssave(species);
                try
                {
                    equilibria << eq;
                    for(Equilibrium::ConstIterator it=eq->begin();it!=eq->end();++it)
                    {
                        species << (*it)->sp;
                    }
                }
                catch(...)
                {
                    esave.xch(equilibria);
                    ssave.xch(species);
                    throw;
                }
            }
            enroll(eq);
        }
    }

}

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Cluster::CallSign = "Cluster";

        Y_PROXY_IMPL(Cluster,my)

        Cluster:: ~Cluster() noexcept
        {
            
        }

        Cluster:: Cluster(Equilibrium &first) :
        Proxy<const ClusterType>(),
        my(),
        next(0),
        prev(0)
        {
            my.link(first);
        }

        void Cluster:: attach(Equilibrium &eq)
        {
            if(latched)                 throw Specific::Exception(CallSign, "latched while attaching '%s'", eq.key().c_str());
            if(my.equilibria->has(eq))  throw Specific::Exception(CallSign, "attaching multiple '%s'", eq.key().c_str());
            my.link(eq);
        }


        bool Cluster:: accepts(const Equilibrium &eq) const noexcept
        {
            for(const SNode *sn=my.species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(eq->search(sp.key())) return true;
            }
            return false;
        }

        bool Cluster:: accepts(const Cluster &cluster) const noexcept
        {
            for(const SNode *sn=my.species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(cluster->species->has(sp)) return true;
            }
            return false;
        }

    }

}
