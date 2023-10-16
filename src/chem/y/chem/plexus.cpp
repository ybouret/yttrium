#include "y/chem/plexus.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        Plexus:: ~Plexus() noexcept
        {
        }

        Plexus:: Plexus(Equilibria    &all,
                        XMLog &        xml) :
        clusters(),
        shared_K()
        {
            Y_XML_SECTION(xml,"Plexus");
            buildClusters(all,xml);
            all.updateMaxSizes();
            shared_K.adjust(all->size(),0);
        }

        Plexus::ConstInterface & Plexus:: surrogate() const noexcept
        {
            return clusters;
        }

    }

}

