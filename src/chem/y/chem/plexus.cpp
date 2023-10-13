#include "y/chem/plexus.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        Plexus:: ~Plexus() noexcept
        {
        }

        Plexus:: Plexus(Equilibria &eqs, XMLog &xml)
        {
            Y_XML_SECTION(xml,"Plexus");
            buildClusters(eqs,xml);
        }

        Plexus::ConstInterface & Plexus:: surrogate() const noexcept
        {
            return clusters;
        }

    }

}

