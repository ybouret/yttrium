#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Charge:: ~Charge() noexcept
        {
        }

        Charge:: Charge() noexcept : z(0)
        {

        }

    }
}


namespace Yttrium
{
    namespace Chemical
    {
        Species:: ~Species() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << sp.key();
            return os;
        }

    }
}
