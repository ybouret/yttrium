#include "y/chem/species/actor.hpp"

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
        xn( nu ),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &a) noexcept :
        Object(),
        sp( a.sp ),
        nu( a.nu ),
        n1( a.n1 ),
        xn( a.xn ),
        next(0),
        prev(0)
        {
        }

        String Actor:: toString() const
        {
            String res;
            if(nu>1)
                res += FormatString("%u",nu);
            res += sp.toString();
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Actor &ac)
        {
            const String s = ac.toString();
            os << s;
            return os;
        }

    }

}

