
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equilibria:: Equilibria() : 
        Proxy<const EqSet>(),
        db()
        {
        }

        Equilibria:: ~Equilibria() noexcept {}

        Equilibria::ConstInterface &Equilibria::surrogate() const noexcept { return db; }

    }

}
