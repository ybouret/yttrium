
#include "y/chemical/reactive/actor.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Actor:: ~Actor() noexcept
        {
        }


        Actor:: Actor(const unsigned _nu, const Species & _sp) :
        nu(_nu),
        sp(_sp),
        xn(nu),
        n1(nu-1),
        next(0),
        prev(0),
        name( (nu>1) ? Formatted::Get("%u",nu) + sp.name : sp.name )
        {
            assert(nu>0);
        }

        Actor:: Actor(const Species & _sp) :
        nu(1),
        sp(_sp),
        xn(1),
        n1(0),
        next(0),
        prev(0),
        name( sp.name )
        {
        }

        const String & Actor:: key() const noexcept
        {
            return name;
        }

    }

}
