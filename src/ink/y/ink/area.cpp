#include "y/ink/area.hpp"

namespace Yttrium
{
    namespace Ink
    {

        Area:: ~Area() noexcept
        {
            Coerce(x)  = 0;
            Coerce(y)  = 0;
            Coerce(w)  = 0;
            Coerce(h)  = 0;
            Coerce(n)  = 0;
            Coerce(xt) = 0;
            Coerce(yt) = 0;
        }

        Area:: Area(const unit_t W, const unit_t H) noexcept :
        x(0),
        y(0),
        w(W),
        h(H),
        n(w*h),
        xt(w-1),
        yt(h-1)
        {
            assert(w>0);
            assert(h>0);
        }

        Area:: Area(const Coord origin, const unit_t W, const unit_t H) noexcept :
        x(origin.x),
        y(origin.y),
        w(W),
        h(H),
        n(w*h),
        xt(x+w-1),
        yt(y+h-1)
        {
            assert(w>0);
            assert(h>0);
        }



        Area:: Area(const Area &a) noexcept :
        x(a.x),
        y(a.y),
        w(a.w),
        h(a.h),
        n(a.n),
        xt(a.xt),
        yt(a.yt)
        {
        }
        
        Coord Area:: lower() const noexcept { return Coord(x,y);   }
        Coord Area:: upper() const noexcept { return Coord(xt,yt); }


        bool Area:: hasSameSizesThan(const Area &a) const noexcept
        {
            return (w == a.w) && (h == a.h);
        }
    }
}

