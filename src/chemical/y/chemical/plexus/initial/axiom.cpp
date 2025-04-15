
#include "y/chemical/plexus/initial/axiom.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {
            const char * const Axiom:: CallSign = "Axiom";


            Axiom:: ~Axiom() noexcept
            {
            }

            Y_PROXY_IMPL(Axiom,my)

            Axiom:: Axiom(const XNode &root, Library &lib) :
            Quantized(),
            Entity( new String() ),
            value(0)
            {
            }
            
        }

    }

}
