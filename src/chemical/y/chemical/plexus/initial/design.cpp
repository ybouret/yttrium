
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Design:: CallSign = "Design";

        Design:: Design(const String * const pName) noexcept :
        Quantized(), Counted(), Entity(pName)
        {

        }
        

        Design:: ~Design() noexcept
        {
        }
    }
}
