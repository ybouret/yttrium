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
        N(all->size())
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


        void Plexus:: graphViz(OutputStream &fp) const
        {
            Enter(fp,"Plexus");

            unsigned ic=0;
            for(const Cluster *cluster=clusters.head;cluster;cluster=cluster->next,++ic)
            {
                fp("subgraph cluster_%u{n",ic);

                cluster->vizSp(fp,0);

                fp("}\n");

            }


            Leave(fp);
        }


    }

}

