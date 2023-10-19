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
#include "y/jive/pattern/matcher.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Plexus:: graphViz() const
        {
            Jive::Matcher         match = "(plexus[:digit:]*[.]png)&";
            GraphViz::Vizible::DotToPng("match.dot", *(match.motif) );

            VFS                  &fs = LocalFS::Instance();
            VFS::Entries          png;
            AutoPtr<VFS::Scanner> scan = fs.openDirectory(".");
            AutoPtr<VFS::Entry>   ep = 0;
            while( (ep=scan->get()).isValid() )
            {
                if(ep->type!=VFS::IsReg) continue;
                //std::cerr << ep->base << std::endl;
                const String baseName = ep->base;
                Jive::Source source( Jive::Module::OpenData(baseName,baseName) );
                if( match.exactly(source) )
                {
                    std::cerr << "(+) " << ep->path << std::endl;
                    png.pushTail(ep.yield());
                }
            }
            while(png.size>0)
            {
                const AutoPtr<VFS::Entry> ep = png.popTail();
                std::cerr << "rm " << ep->path << std::endl;
            }
        }

    }

}

