
#include "y/chemical/plexus/initial/wish.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {

            const char * const Wish:: CallSign = "Wish";


            Wish:: ~Wish() noexcept
            {
            }

            Wish:: Wish() :
            Entity( new String() )
            {

            }
        }
        
    }

}
