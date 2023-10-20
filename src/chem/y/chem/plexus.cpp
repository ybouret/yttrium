#include "y/chem/plexus.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        Plexus:: ~Plexus() noexcept
        {
        }

        Plexus:: Plexus(Equilibria    &all,
                        XMLog &        xml,
                        const double   t0) :
        clusters(),
        shared_K(),
        primaryN(all->size()),
        maxOrder(0)
        {
            Y_XML_SECTION(xml,"Plexus");
            buildClusters(all,xml);
            all.updateMaxSizes();
            shared_K.adjust(all->size(),0);
            updateK(t0);
        }

        const Readable<xreal> & Plexus:: K() const noexcept
        {
            return shared_K;
        }

        const Readable<xreal> & Plexus:: K(double t)
        {
            updateK(t);
            return shared_K;
        }

        void Plexus:: updateK(const double t)
        {
            for(Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
            {
                cluster->updateK(shared_K,t);
            }
        }



        Plexus::ConstInterface & Plexus:: surrogate() const noexcept
        {
            return clusters;
        }


        void Plexus:: graphViz(OutputStream &fp, const size_t order) const
        {
            assert(order>=1);
            Enter(fp,"Plexus");

            unsigned ic=0;
            for(const Cluster *cluster=clusters.head;cluster;cluster=cluster->next,++ic)
            {
                if(order<=cluster->meg->size())
                {
                    fp("subgraph cluster_%u{\n",ic);
                    cluster->viz(fp,order);
                    fp("}\n");
                }
            }

            Leave(fp);
        }

    }

}


#include "y/vfs/local-fs.hpp"
#include "y/jive/pattern/vfs.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Plexus:: graphViz() const
        {
            static VFS   &theFS = LocalFS::Instance();
            Jive::Matcher match = "(plexus[:digit:]*[.]png)&";//GraphViz::Vizible::DotToPng("match.dot", *(match.motif) );
            VFS::Entries  elist;
            Jive::VirtualFileSystem::Find(theFS,".", elist, match);
            while(elist.size>0)
            {
                const AutoPtr<VFS::Entry> ep = elist.popTail();
                std::cerr << "rm " << ep->path << std::endl;
            }


        }

    }

}

