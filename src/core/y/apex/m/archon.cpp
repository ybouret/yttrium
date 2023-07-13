
#include "y/apex/m/archon.hpp"

namespace Yttrium
{
    namespace Apex
    {
        const char * const Archon:: CallSign = "Apex::Archon";
        
        Archon:: ~Archon() noexcept
        {
        }

        Archon:: Archon() noexcept : Singleton<Archon>()
        {

        }


    }

}
