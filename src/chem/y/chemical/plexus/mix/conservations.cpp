#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Mix::Conservations");
            Coerce(auth)  = new Conservation::Authority(*this,xml);
            Coerce(genus) = new Genus(auth,my.species);

            Y_XMLOG(xml,"conserved=" << genus->conserved.list);
            Y_XMLOG(xml,"unbounded=" << genus->unbounded.list);

        }
    }

}
