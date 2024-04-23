#include "y/chemical/reactive/cluster.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(Equilibria        &eqs,
                          const Fragment    &fragment,
                          const Constants   &topK,
                          XMLog             &xml) :
        ClusterCombinatorics(eqs,fragment,topK,xml),
        next(0),
        prev(0)
        {

        }

        Cluster::  ~Cluster() noexcept {}


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
                for(const Conservation::Law *law=laws->head;law;law=law->next)
                {
                    law->viz(fp);
                }
            }

            fp << "}\n";
        }


    }

}
