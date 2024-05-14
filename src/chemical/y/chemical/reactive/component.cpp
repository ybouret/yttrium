
#include "y/chemical/reactive/component.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Component:: Component(const int coef, const Species &spec) noexcept :
        nu(coef),
        xn(nu),
        sp(spec)
        {
            assert(0!=nu);
        }

        Component:: ~Component() noexcept
        {
        }

        Component:: Component(const Component &other) noexcept :
        nu(other.nu),
        xn(other.xn),
        sp(other.sp)
        {
        }

        const String & Component:: key() const noexcept
        {
            return sp.name;
        }


    }

}
