#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(const Equilibrium &eq,
                          const Constants   &topK) :
        EList(),
        sharedK(topK),
        species(),
        Nu(),
        Qm(),
        spset(),
        eqset(),
        eqfmt(),
        spfmt(),
        claws(),
        cgrps(),
        blend(),
        next(0),
        prev(0)
        {
            (*this) << eq;
        }

        Cluster::  ~Cluster() noexcept {}


        bool Cluster:: sharesSpeciesWith(const Equilibrium &rhs) const noexcept
        {
            for(const ENode *mine=head;mine;mine=mine->next)
            {
                const Equilibrium &lhs = **mine;
                if( lhs.sharesSpeciesWith(rhs) ) return true;
            }
            return false;
        }

        bool Cluster:: sharesSpeciesWith(const Cluster &other) const noexcept
        {
            for(const ENode *mine=head;mine;mine=mine->next)
            {
                if(other.sharesSpeciesWith(**mine)) return true;
            }
            return false;
        }

        void Cluster:: getK(const Real t)
        {
            Writable<XReal> &K = *sharedK;
            for(const ENode *node=head;node;node=node->next)
            {
                Equilibrium &eq = Coerce(**node);
                K[eq.indx[TopLevel]] = eq.K(t);
            }
        }


        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            os << "<Cluster species='" << cl.species.size << "' eqs='" << cl.size << "'>" << std::endl;
            for(const ENode *node=cl.head;node;node=node->next)
            {
                cl.eqfmt.display(os << "  ", **node) << std::endl;
            }
            os << "  (*) " << cl.species << std::endl;
            return os << "<Cluster/>";
        }


        void Cluster:: viz(OutputStream &fp, const size_t order, const size_t clusterIndex) const
        {
            assert(order>0);
            assert(order<=blend.size());
            const EList &eqs = blend[order];
            fp("subgraph cluster_%lu {\n", static_cast<unsigned long>(clusterIndex));
            for(const SNode *sn = species.head;sn;sn=sn->next)
            {
                (**sn).viz(fp);
            }

            for(const ENode *en = eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.viz(fp,SubLevel);
                eq.vizLink(fp,SubLevel);
            }

            if(1==order)
            {
                size_t indx=0;
                for(const Conservation::Law *law=claws->head;law;law=law->next)
                {
                    law->viz(fp,++indx);
                }
            }

            fp << "}\n";




        }


    }

}
