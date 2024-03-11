
#include "y/ink/metrics.hpp"


namespace Yttrium
{
    namespace Ink
    {

        Metrics:: ~Metrics() noexcept
        {
            Coerce(s) = 0;
            Coerce(b) = 0;
        }

        Metrics:: Metrics(const unit_t    W,
                          const unit_t    H,
                          const unsigned  bs) noexcept :
        Area(W,H),
        s(w*bs),
        b(bs),
        zfh(h),
        zfw(w)
        {
            assert(b>0);
        }

        Metrics:: Metrics(const Metrics &other) noexcept :
        Area(other),
        s(other.s),
        b(other.b),
        zfh(other.zfh),
        zfw(other.zfw)
        {
        }



        Metrics:: Metrics(const Metrics &other, const unsigned bs) noexcept :
        Area(other),
        s(w*bs),
        b(bs),
        zfh(other.zfh),
        zfw(other.zfw)
        {
            assert(b>0);
        }



    }

}
