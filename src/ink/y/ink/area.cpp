#include "y/ink/area.hpp"

namespace Yttrium
{
    namespace Ink
    {

        Area:: ~Area() noexcept
        {
            Coerce(x) = 0;
            Coerce(y) = 0;
            Coerce(w) = 0;
            Coerce(h) = 0;
            Coerce(n) = 0;
        }

        Area:: Area(const unit_t W, const unit_t H) noexcept :
        x(0),
        y(0),
        w(W),
        h(H),
        n(w*h)
        {
            assert(w>0);
            assert(h>0);
        }

        Area:: Area(const Area &a) noexcept :
        x(a.x),
        y(a.y),
        w(a.w),
        h(a.h),
        n(a.n)
        {
        }
        


    }
}

