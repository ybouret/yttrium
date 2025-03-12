
#include "y/chemical/reactive/equilibria.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        EquilibriaType:: EquilibriaType() : EquilibriaSet(), Fragment()
        {
        }

        EquilibriaType:: ~EquilibriaType() noexcept
        {
        }
    }

}

namespace Yttrium
{
    namespace Chemical
    {

        Equilibria:: ~Equilibria() noexcept
        {
        }

        Equilibria:: Equilibria() :
        Proxy<const EquilibriaType>(),
        db()
        {
        }
        


    }

}


