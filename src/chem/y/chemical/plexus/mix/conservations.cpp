#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConservations(XMLog &xml)
        {
            Coerce(laws) = new Conservation::Laws(*this,xml);
            
        }
    }

}
