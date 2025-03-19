
#include "y/chemical/plexus/cluster/content.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterContent:: ClusterContent(Equilibrium &first)  :
        Fragment(), equilibria(), species(), sformat()
        {
            attach(first);
        }

        ClusterContent:: ~ClusterContent() noexcept
        {
        }

        void ClusterContent:: updateFmt() noexcept
        {
            for(const SNode *sn=species->head;sn;sn=sn->next)
                sformat.enroll(**sn);
        }

        void ClusterContent:: attach(Equilibrium &eq)
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
            updateFmt();
            assert(equilibria->size>0);
            assert(species->size>0);
        }

        void ClusterContent:: fusion(ClusterContent &other) noexcept
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
                enroll(**en);
            updateFmt();
        }


        bool ClusterContent:: accepts(const Equilibrium &eq) const noexcept
        {
            for(const SNode *sn=species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if( eq->search( sp.key() ) ) return true;
            }
            return false;
        }

        bool ClusterContent:: accepts(const ClusterContent &other) const noexcept
        {
            for(const SNode *sn=species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(other.species->has(sp)) return true;
            }
            return false;
        }

        std::ostream & operator<<(std::ostream &os, const ClusterContent &cc)
        {
            os << "{ species=" << cc.species << std::endl;
            for(const ENode *en=cc.equilibria->head;en;en=en->next)
            {
                cc.display(os << '\t',**en) << std::endl;
            }
            os << "}";
            return os;
        }

    }

}
