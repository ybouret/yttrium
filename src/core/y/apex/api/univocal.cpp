
#include "y/apex/api/univocal.hpp"

namespace Yttrium
{
    namespace Apex
    {

        bool Univocal:: MustSwapSigns(const size_t p, const size_t n, const SignType f) noexcept
        {
            assert(f!=__Zero__);
            return n>p || (n==p && f == Negative);
        }

    }

}

