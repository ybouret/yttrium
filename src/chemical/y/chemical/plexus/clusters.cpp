#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Clusters,my)

        Clusters:: ~Clusters() noexcept
        {
            
        }

        
        Clusters:: Clusters(XMLog      &xml,
                            Equilibria &eqs,
                            const xreal_t t0) :
        my(),
        tlK(),
        K(tlK)
        {
            const size_t ini = eqs->size();
            Y_XML_SECTION_OPT(xml, "Clusters", "|eqs|=" << ini);

            {
                ClusterBuilder cls(xml,eqs);
                for(const ClusterKnot *cl=cls->head;cl;cl=cl->next)
                {
                    const ClusterContent::Pointer clc( &Coerce(**cl) );
                    my.pushTail( new Cluster(xml,clc,eqs,tlK) );
                }
            }
            Y_XML_COMMENT(xml, "|eqs|=" << eqs->size() << " from " << ini);
            tlK.adjust(eqs->size(),0);
            (void) (*this)(t0);
        }

        const XReadable & Clusters:: operator()(const xreal_t t0)
        {

            for(Cluster *cl=my.head;cl;cl=cl->next)
            {
                for(ENode *en=(*cl)->equilibria->head;en;en=en->next)
                {
                    Equilibrium &eq = **en;
                    eq(tlK,TopLevel) = eq.K(t0);
                }
            }
            return K;
        }

    }

}

