
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Design:: CallSign = "Design";
        const char * const Design:: LogoExpr  = "[.][[:word:]_]+";
        const char * const Design:: Logo     = "Logo";

        Design:: Design(const String * const pName) noexcept :
        Quantized(), Counted(), Entity(pName)
        {

        }
        

        Design:: ~Design() noexcept
        {
        }
    }
}
