
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConfiguration(XMLog &xml,Equilibria &eqs)
        {
            buildConservations(xml);
            buildCombinatorics(xml,eqs);
        }
    }

}
