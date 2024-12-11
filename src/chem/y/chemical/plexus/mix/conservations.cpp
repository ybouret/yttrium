#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Mix::Conservations");
            Coerce(laws) = new Conservation::Laws(*this,xml);
            
        }
    }

}
