#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"
#include "y/stream/libc/output.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Clusters,my)

        Clusters:: ~Clusters() noexcept
        {

        }

        static inline
        void CollectListing(Listing &target, const SNode * node)
        {
            for(;node;node=node->next)
            {
                const Species &sp = **node;
                if(0!=sp.z) Coerce(target.charged) << sp; else Coerce(target.neutral) << sp;
            }
        }

        const char * const Clusters:: CallSign = "Chemical::Clusters";

        Clusters:: Clusters(XMLog      &   xml,
                            Equilibria &   eqs,
                            const Library &lib,
                            const xreal_t  t0) :
        my(),
        tlK(),
        K(tlK),
        primary(eqs->size()),
        reactive(),
        conserved(),
        unbounded(),
        spectator(),
        maxOrder(0)
        {
            assert(!eqs.latched);
            Y_XML_SECTION_OPT(xml, "Clusters", "|eqs|=" << primary);
            if(!lib.latched) throw Specific::Exception(CallSign, "%s must be latched", Library::CallSign);

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
                    Coerce(reactive) << **sn;
                }
            }
            MergeSort::Call( Coerce(reactive), MetaList<SList>::Compare );

            // collect limited
            for(const Cluster *cl=my.head;cl;cl=cl->next)
            {
                CollectListing(Coerce(conserved),cl->conserved.head);
                CollectListing(Coerce(unbounded),cl->unbounded.head);
            }
            Coerce(conserved).sort();
            Coerce(unbounded).sort();
            
            assert(conserved.size()+unbounded.size()==reactive.size);


            // collect spectators
            for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
            {
                const Species &sp = **it; if(reactive.has(sp)) continue;
                if(0==sp.z)
                    Coerce(spectator.neutral) << sp;
                else
                    Coerce(spectator.charged) << sp;
            }
            Coerce(spectator).sort();



            // summary
            {
                Y_XML_SECTION(xml, "Summary");
                Y_XML_COMMENT(xml, "|eqs|       = " << eqs->size() << " from " << primary);
                Y_XML_COMMENT(xml, "|reactive|  = " << reactive.size);
                Y_XML_COMMENT(xml, "|spectator| = " << spectator.size());
                Y_XML_COMMENT(xml, "maxOrder    = " << maxOrder );
            }
            // prepare constants
            tlK.adjust(eqs->size(),0);
            (void) (*this)(t0);
            assert(!eqs.latched);
            eqs.latch();
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

        const Conservation::Law * Clusters:: preserving(Lingo::Matching &m) const
        {

            for(const Cluster *curr=my.head;curr;curr=curr->next)
            {
                const Conservation::Law *law = curr->preserving(m);
                if(0!=law)
                {

                    for(curr=curr->next;curr;curr=curr->next) {
                        if(0!=curr->preserving(m)) throw Specific::Exception(CallSign,"multiple preserving pattern");
                    }
                    return law;
                }
            }

            return 0;
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

