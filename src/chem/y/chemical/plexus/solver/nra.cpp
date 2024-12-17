
#include "y/chemical/plexus/solver.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        xReal Solver:: buildNRA(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml,NRA);

            return f0;
        }

        
    }

}
