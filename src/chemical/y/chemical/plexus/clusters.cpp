#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"
#include "y/stream/libc/output.hpp"

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
        K(tlK),
        species(),
        maxOrder(0)
        {
            const size_t ini = eqs->size();
            Y_XML_SECTION_OPT(xml, "Clusters", "|eqs|=" << ini);

            {
                ClusterBuilder cls(xml,eqs);
                unsigned       idx = 1;
                for(const ClusterKnot *cl=cls->head;cl;cl=cl->next)
                {
                    const ClusterContent::Pointer clc( &Coerce(**cl) );
                    Coerce(my.pushTail( new Cluster(xml,clc,eqs,tlK) )->uuid) = idx++;
                }
            }

            // collect max order and species
            for(const Cluster *cl=my.head;cl;cl=cl->next)
            {
                InSituMax( Coerce(maxOrder), cl->order.size() );
                for(const SNode *sn=(*cl)->species->head;sn;sn=sn->next)
                {
                    Coerce(species) << **sn;
                }
            }
            MergeSort::Call( Coerce(species), MetaList<SList>::Compare );


            // summary
            Y_XML_COMMENT(xml, "|eqs|     = " << eqs->size() << " from " << ini);
            Y_XML_COMMENT(xml, "|species| = " << species.size);
            Y_XML_COMMENT(xml, "maxOrder  = " << maxOrder );

            // prepare constants
            tlK.adjust(eqs->size(),0);
            (void) (*this)(t0);
        }

        const XReadable & Clusters:: operator()(const xreal_t t0)
        {

            for(Cluster *cl=my.head;cl;cl=cl->next)
            {
                for(ENode *en=(*cl)->equilibria->head;en;en=en->next)
                {
                    Equilibrium &eq  = Coerce(**en);
                    eq(tlK,TopLevel) = eq.K(t0);
                }
            }
            return K;
        }


    }

}


#include "y/lingo/vfs/find.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace Lingo;

        void Clusters:: graphViz(const String &baseName) const
        {
            {
                const String rx = Pattern::ToRegularExpression(baseName) + "[:digit:]+[.](dot|png)";
                LocalFileSystem::TryRemoveBaseName::In(".",rx);
            }

            for(size_t i=1;i<=maxOrder;++i)
            {
                const String fn = baseName + Formatted::Get("%u", unsigned(i)) + ".dot";
                {
                    OutputFile fp(fn);
                    GraphViz::Vizible::Enter(fp,"G");
                    for(Cluster *cl=my.head;cl;cl=cl->next)
                    {
                        cl->viz(fp,i);
                        fp << '\n';
                    }
                    GraphViz::Vizible::Leave(fp);
                }
                GraphViz::Vizible::RenderPNG(fn,false);
            }
        }
    }

}

