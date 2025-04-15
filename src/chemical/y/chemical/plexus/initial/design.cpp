
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {
            const char * const Design:: CallSign = "Design";
            const char * const Design:: LogoExpr  = "[.][[:word:]_]+";
            const char * const Design:: Logo     = "Logo";
            
            Design:: Design():
            Quantized(), Counted(), Entity( new String() )
            {
                
            }
            
            
            Design:: ~Design() noexcept
            {
            }
        }
    }
}
