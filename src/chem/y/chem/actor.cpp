#include "y/chem/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Actor:: ~Actor() noexcept {}

        Actor:: Actor(const Species &which,
                      const unsigned coeff) noexcept :
        sp( which ),
        nu( coeff ),
        n1( nu-1  ),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &a) noexcept :
        sp( a.sp ),
        nu( a.nu ),
        n1( a.n1 ),
        next(0),
        prev(0)
        {
        }

        String Actor:: toString() const
        {
            String res;
            if(nu>1)
                res += FormatString("%u*",nu);
            res += sp.name;
            return res;
        }

    }

}

