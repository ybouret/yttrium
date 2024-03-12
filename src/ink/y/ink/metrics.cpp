
#include "y/ink/metrics.hpp"


namespace Yttrium
{
    namespace Ink
    {

        Metrics:: ~Metrics() noexcept
        {
            Coerce(s)   = 0;
            Coerce(bpp) = 0;
        }

        Metrics:: Metrics(const unit_t    W,
                          const unit_t    H,
                          const unsigned  bs) noexcept :
        Area(W,H),
        s(w*bs),
        bpp(bs),
        zfh(h),
        zfw(w)
        {
            assert(bpp>0);
        }

        Metrics:: Metrics(const unit_t    W,
                          const unit_t    H,
                          const unsigned  B,
                          const unit_t    S) noexcept :
        Area(W,H),
        s(S),
        bpp(B),
        zfh(h),
        zfw(w)
        {
            assert(bpp>0);
            assert(s>=w*bpp);
        }

        Metrics:: Metrics(const Metrics &other) noexcept :
        Area(other),
        s(other.s),
        bpp(other.bpp),
        zfh(other.zfh),
        zfw(other.zfw)
        {
        }



        Metrics:: Metrics(const Metrics &other, const unsigned bs) noexcept :
        Area(other),
        s(w*bs),
        bpp(bs),
        zfh(other.zfh),
        zfw(other.zfw)
        {
            assert(bpp>0);
        }



    }

}
