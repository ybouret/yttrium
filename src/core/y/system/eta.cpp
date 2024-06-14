
#include "y/system/eta.hpp"
#include "y/system/hrt.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    ETA:: ETA() :
    wallTime(),
    iniTicks(0),
    nowTicks(0),
    fraction(0),
    ellapsed(0)
    {

    }

    ETA:: ~ETA() noexcept {}


    void ETA:: start() {
        Coerce(iniTicks) = WallTime::LockedTicks();
        Coerce(nowTicks) = iniTicks;
        Coerce(fraction) = 0;
        Coerce(ellapsed) = 0;
    }

    void ETA:: getEllapsed()
    {
        Coerce(ellapsed) = static_cast<double>( wallTime( (Coerce(nowTicks) = WallTime::LockedTicks() ) - iniTicks  ) );
    }

    double ETA:: getEstimate() const noexcept
    {
        Coerce(fraction) = Clamp<double>(0,fraction,1);
        const double done = fraction;
        const double todo = 1.0l - done;
        const double num  = todo * ellapsed;
        if( num >= done * HRT::MaxSeconds )
        {
            return HRT::MaxSeconds;
        }
        else
        {
            return (num/done);
        }
    }

}

