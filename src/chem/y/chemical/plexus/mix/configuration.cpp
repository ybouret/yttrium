
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConfiguration(Equilibria &eqs, XMLog &xml)
        {
            buildConservations(xml);
            buildCombinatorics(eqs,xml);
        }
    }

}
