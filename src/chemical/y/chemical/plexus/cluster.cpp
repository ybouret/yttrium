
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

        void ClusterType:: attach(Equilibrium &eq)
        {
            assert(equilibria->isStrictlySortedBy(SubEList::Compare));
            assert(species->isStrictlySortedBy(SubSList::Compare));
            assert(!equilibria->has(eq));

            if(eq->size()<=0)
                throw Specific::Exception(Cluster::CallSign,"empty equilibrium '%s'", eq.key().c_str());

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
                    assert(equilibria->isStrictlySortedBy(SubEList::Compare));
                    assert(species->isStrictlySortedBy(SubSList::Compare));
                }
                catch(...)
                {
                    esave.xch(equilibria);
                    ssave.xch(species);
                    throw;
                }
            }
            enroll(eq);

            assert(equilibria->size>0);
            assert(species->size>0);
        }

        void ClusterType:: fusion(ClusterType &other) noexcept
        {
            assert(      equilibria->isStrictlySortedBy(SubEList::Compare));
            assert(         species->isStrictlySortedBy(SubSList::Compare));
            assert(other.equilibria->isStrictlySortedBy(SubEList::Compare));
            assert(other.   species->isStrictlySortedBy(SubSList::Compare));

            equilibria.fusion(other.equilibria);
            species.fusion(other.species);

            assert(      equilibria->isStrictlySortedBy(SubEList::Compare));
            assert(         species->isStrictlySortedBy(SubSList::Compare));

            for(const ENode *en=equilibria->head;en;en=en->next)
            {
                enroll(**en);
            }
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
            my.attach(first);
        }

        void Cluster:: attach(Equilibrium &eq)
        {
            if(latched)                 throw Specific::Exception(CallSign, "latched while attaching '%s'", eq.key().c_str());
            if(my.equilibria->has(eq))  throw Specific::Exception(CallSign, "attaching multiple '%s'", eq.key().c_str());
            my.attach(eq);
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


