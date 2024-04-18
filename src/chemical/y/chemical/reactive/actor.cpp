
#include "y/chemical/reactive/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actor:: ~Actor() noexcept {}

        Actor:: Actor(const unsigned coef,
                      const Species &spec) noexcept :
        Object(),
        sp(spec),
        nu(coef),
        xnu(nu),
        nu1(nu-1),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &a) noexcept :
        Object(),
        sp(a.sp),
        nu(a.nu),
        xnu(a.xnu),
        nu1(a.nu1),
        next(0),
        prev(0)
        {

        }



    }

}
