
#include "y/chemical/reactive/clusters.hpp"
#include "y/chemical/reactive/equilibria/batches.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Clusters:: Clusters(Equilibria      &eqs,
                            const Constants &topK,
                            XMLog           &xml) :
        clusters(),
        sharedK(topK),
        groups(),
        maxSPC(0),
        maxCPG(0),
        maxSPG(0),
        maxDEG(0),
        maxCPC(0)
        {
            Y_XML_SECTION(xml, "Chemical::Clusters" );

            //__________________________________________________________________
            //
            // create batches
            //
            //__________________________________________________________________
            const Batches batches(eqs);

            //__________________________________________________________________
            //
            // create one cluster per batch
            //__________________________________________________________________
            for(const Batch *batch=batches.head;batch;batch=batch->next)
            {
                const Cluster *cl = clusters.pushTail( new Cluster(eqs,*batch,sharedK,xml) );
                for(const Conservation::Group *grp=cl->groups->head;grp;grp=grp->next)
                {
                    const Conservation::Group &g = *grp;
                    Coerce(maxCPG) = Max(g->size,maxCPG);
                    Coerce(maxSPG) = Max(maxSPG,g.species.size);
                    Coerce(groups) << g;
                }
                Coerce(maxSPC) = Max(maxSPC,cl->species.size);
                Coerce(maxDEG) = Max(maxDEG,cl->blend.size());
                Coerce(maxCPC) = Max(maxCPC,cl->controllers.size);
            }

            //__________________________________________________________________
            //
            // finalize: adjust top-level constants
            //__________________________________________________________________
            sharedK->adjust(eqs->size(),0);
            {
                Y_XML_SECTION(xml, "Summary");
                Y_XMLOG(xml, "  (*) #Cluster                     : " << clusters.size);
                Y_XMLOG(xml, "  (*) Max Species Per Cluster      : " << maxSPC);
                Y_XMLOG(xml, "  (*) Max Degree  Per Cluster      : " << maxDEG);
                Y_XMLOG(xml, "  (*) #Conservation Group          : " << groups.size);
                for(const GNode *gn=groups.head;gn;gn=gn->next)
                {
                    Y_XML_SECTION(xml, "Group");
                    const Conservation::Group &g = **gn;
                    for(const Conservation::LawNode *ln=g->head;ln;ln=ln->next)
                    {
                        Y_XMLOG(xml,"   |-" << **ln);
                    }
                }
                Y_XMLOG(xml, "  (*) Max Conservation Per Group   : " << maxCPG);
                Y_XMLOG(xml, "  (*) Max Species      Per Group   : " << maxSPG);
                Y_XMLOG(xml, "  (*) Max Controllers  Per Cluster : " << maxCPC);

            }


        }

        Clusters::ConstInterface & Clusters:: surrogate() const noexcept
        {
            return clusters;
        }


        Clusters:: ~Clusters() noexcept
        {
        }

        const Readable<xreal_t> & Clusters :: K(const real_t t)
        {
            for(Cluster *cl=clusters.head;cl;cl=cl->next)
                cl->getK(t);
            return *sharedK;
        }

    }

}

#include "y/vfs/local-fs.hpp"
#include "y/jive/pattern/vfs.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Clusters:: graphViz(const String &root) const
        {
            VFS          &fs = LocalFS::Instance();

            //------------------------------------------------------------------
            //
            // remove 'root[:digit:]*.(dot|png)'
            //
            //------------------------------------------------------------------
            {
                //--------------------------------------------------------------
                // collect entries
                //--------------------------------------------------------------
                VFS::Entries entries;
                {
                    const String  rx = Jive::Pattern::ToRegExp(root) +"[:digit:]*[.](dot|png)";
                    Jive::Matcher match(rx); //GraphViz::Vizible::DotToPng("motif.dot", *match.motif);
                    Jive::VirtualFileSystem::Find(fs, ".", entries, match, VFS::Entry::Path);
                }

                //--------------------------------------------------------------
                //
                //--------------------------------------------------------------
                for(const VFS::Entry *ep=entries.head;ep;ep=ep->next)
                {
                    std::cerr << "--> " << *ep << std::endl;
                    fs.tryRemoveFile(ep->path);
                }
            }

            //------------------------------------------------------------------
            //
            // one graph per degreee
            //
            //------------------------------------------------------------------
            for(size_t deg=1;deg<=maxDEG;++deg)
            {

                const String dotName = root + Formatted::Get("%u.dot",unsigned(deg));
                {
                    OutputFile fp(dotName);
                    GraphViz::Vizible::Enter(fp, "G");
                    size_t clusterIndex = 1;
                    for(const Cluster *cl=clusters.head;cl;cl=cl->next,++clusterIndex)
                    {
                        if(deg<=cl->blend.size())
                        {
                            cl->viz(fp,deg,clusterIndex);
                        }
                    }
                    GraphViz::Vizible::Leave(fp);
                }
                std::cerr << " (*) Rendering '" << dotName << "'..." << std::endl;
                GraphViz::Vizible::RenderPNG(dotName,false);
            }
        }


    }

}
