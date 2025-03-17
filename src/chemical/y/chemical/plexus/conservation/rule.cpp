
#include "y/chemical/plexus/conservation/rule.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Rule:: ~Rule() noexcept
            {
            }

            Rule:: Rule() noexcept :
            Actors(Actor::AsConcentration),
            next(0),
            prev(0)
            {
            }

        }

    }

}

