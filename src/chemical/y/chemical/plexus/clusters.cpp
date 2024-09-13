
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Clusters:: ~Clusters() noexcept
        {
        }

        Clusters::ConstInterface & Clusters:: surrogate() const noexcept
        {
            return cls;
        }

        Clusters:: Clusters(Equilibria &eqs, XMLog &xml) :
        Quantized(),
        Counted(),
        Proxy<const Cluster::List>(),
        cls(),
        shK(),
        maxEPC(0),
        maxSPC(0),
        maxLPG(0),
        maxORD(0),
        species()
        {
            Y_XML_SECTION(xml, "Clusters");

            //------------------------------------------------------------------
            //
            //
            // build clusters
            //
            //
            //------------------------------------------------------------------
            for(Equilibria::ConstIterator it=eqs->begin(); it !=eqs->end(); ++it)
            {
                const Equilibrium &eq = **it;

                //--------------------------------------------------------------
                // look for an accepting cluster
                //--------------------------------------------------------------
                for(Cluster *cl=cls.head;cl;cl=cl->next)
                {
                    if(cl->accept(eq))
                    {
                        *cl << eq;
                        goto CHECK_FUSION;
                    }
                }

                //--------------------------------------------------------------
                // create new cluster
                //--------------------------------------------------------------
                cls.pushTail(new Cluster(eq) );
                continue;

                //--------------------------------------------------------------
                // check fusion
                //--------------------------------------------------------------
            CHECK_FUSION:
                Cluster::List store;
                while( cls.size > 0 )
                {
                    AutoPtr<Cluster> mine = cls.popHead();
                    for(Cluster *cl=store.head;cl;cl=cl->next)
                    {
                        if(cl->accept(*mine))
                        {
                            (*cl).mergeTail( *(mine.yield()) );
                            break;
                        }
                    }
                    if(mine.isValid()) store.pushTail( mine.yield() );
                }
                cls.swapWith(store);
            }

            //------------------------------------------------------------------
            //
            //
            // compile clusters
            //
            //
            //------------------------------------------------------------------
            size_t total = 0;
            for(Cluster *cl = cls.head; cl; cl=cl->next )
            {
                total += cl->compile(xml,eqs,shK).size;
                Coerce(maxEPC) = Max(maxEPC,cl->size);
                Coerce(maxSPC) = Max(maxSPC,cl->species.size);
                Coerce(maxORD) = Max(maxORD,cl->maxOrder());

                if(cl->laws.isValid())
                    Coerce(maxLPG) = Max(maxLPG,cl->laws->maxGroupSize);
                Coerce(species) << cl->species;

            }
            Indexed::SortBy<TopLevel>::Using( Coerce(species) );

            //------------------------------------------------------------------
            //
            //
            // Allocate the shared constants
            //
            //
            //------------------------------------------------------------------
            shK.adjust(total,0);

            //------------------------------------------------------------------
            //
            //
            // compile clusters
            //
            //
            //------------------------------------------------------------------
            Y_XML_SECTION(xml, "Metrics");
            Y_XMLOG(xml, "#equilibria                = " << total);
            Y_XMLOG(xml, "#active species            = " << species.size);
            Y_XMLOG(xml, "max Equilibria Per Cluster = " << maxEPC);
            Y_XMLOG(xml, "max Species    Per Cluster = " << maxSPC);
            Y_XMLOG(xml, "max Laws       Per Group   = " << maxLPG);
            Y_XMLOG(xml, "max Combinations Order     = " << maxORD);

        }


        const XReadable & Clusters:: K(const xreal_t t)
        {
            for(Cluster *cl = cls.head; cl; cl=cl->next )
            {
                for(ENode *en=cl->head;en;en=en->next)
                {
                    Equilibrium &eq = Coerce(**en);
                    shK[eq.indx[TopLevel]] = eq.K(t);
                }
            }
            return shK;
        }


        void Clusters:: show(std::ostream &os) const
        {
            for(const Cluster *cl = cls.head; cl; cl=cl->next )
            {
                cl->show(os);
            }
        }

        void Clusters:: showK(std::ostream &os) const
        {
            for(const Cluster *cl = cls.head; cl; cl=cl->next )
            {
                cl->show(os,shK);
            }
        }

    }

}

#include "y/vfs/local/fs.hpp"
#include "y/jive/pattern/vfs.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        void Clusters:: graphViz(OutputStream &fp, const size_t ord) const
        {
            GraphViz::Vizible::Enter(fp,"G");
            size_t cid = 0;
            for(const Cluster *cl=cls.head;cl;cl=cl->next,++cid)
            {
                cl->viz(ord, fp, cid);
            }
            GraphViz::Vizible::Leave(fp);

        }

        void Clusters:: render(const String &rootName, const size_t ord) const
        {

            const String dotFile = rootName + Formatted::Get("%u",unsigned(ord)) + ".dot";
            GraphViz::Vizible::DotToPngEx(dotFile,*this,ord);

        }

        void Clusters:: render(const String &rootName) const
        {
            {
                const String   rxp   = rootName + "[:digit:]+[.](png|dot)";
                Jive::Matcher  match = rxp;  //GraphViz::Vizible::DotToPng("matching.dot", *match.motif);
                VFS::Entries   entries;
                VFS           &vfs = Jive::VirtualFileSystem::List(entries, LocalFS::Instance(), "", rxp, VFS::Entry::Base);
                for(const VFS::Entry *ep=entries.head;ep;ep=ep->next)
                {
                    std::cerr << " (-) " << ep->path << std::endl;
                    vfs.tryRemoveFile(ep->path);
                }
            }

            for(size_t i=1;i<=maxORD;++i)
            {
                render(rootName,i);
            }

        }



    }

}
