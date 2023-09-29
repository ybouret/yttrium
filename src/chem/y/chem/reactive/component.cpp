#include "y/chem/reactive/component.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Component:: ~Component() noexcept
        {
        }

        Component:: Component(const Component &cc) noexcept :
        sp(cc.sp),
        nu(cc.nu)
        {
            assert(0!=nu);
        }

        Component:: Component(const Species &which,
                              const int      coeff) noexcept :
        sp(which),
        nu(coeff)
        {
            assert(0!=nu);
        }

        const String & Component:: key() const noexcept { return sp.name; }
        
    }

}
