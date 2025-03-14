#include "y/chemical/plexus/cluster/type.hpp"
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
                throw Specific::Exception("ClusterType","attach empty equilibrium '%s'", eq.key().c_str());

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
