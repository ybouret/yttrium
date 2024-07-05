
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Species:: ~Species() noexcept {
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            return os<<sp.name;
        }

    }

}

