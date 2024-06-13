
#include "y/system/eta.hpp"
#include "y/system/hrt.hpp"

namespace Yttrium
{
    ETA:: ETA() :
    wallTime(),
    iniTicks(0),
    nowTicks(0),
    procTime(0)
    {

    }

    ETA:: ~ETA() noexcept {}


    void ETA:: start() {
        Coerce(iniTicks) = WallTime::LockedTicks();
        Coerce(nowTicks) = iniTicks;
        Coerce(procTime) = 0;
    }

    long double ETA:: ellapsed()
    {
        return ( Coerce(procTime) = wallTime( (Coerce(nowTicks) = WallTime::LockedTicks() ) - iniTicks  ) );
    }

    double ETA:: estimate(const long double done, const long double ella) const noexcept
    {
        const long double todo = 1.0l - done;
        const long double num  = todo * ella;
        if( num >= done * HRT::MaxSeconds )
        {
            return HRT::MaxSeconds;
        }
        else
        {
            return double(num/done);
        }
    }

}

