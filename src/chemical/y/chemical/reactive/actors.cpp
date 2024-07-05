
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: ~Actors() noexcept {}

        Actors:: Actors() noexcept :
        Entity(""),
        Proxy<const Actor::List>(),
        actors() {}

        Actors::ConstInterface &Actors:: surrogate() const noexcept { return actors; }

        std::ostream & operator<<(std::ostream &os, const Actors &ac)
        {
            return os<<ac.name;
        }

    }

}


