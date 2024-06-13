
#include "y/system/hrt.hpp"
#include "y/type/utils.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>

namespace Yttrium
{
    HRT:: ~HRT() noexcept
    {
    }

    HRT:: HRT(double seconds) noexcept : fmt()
    {
        memset(fmt,0,sizeof(fmt));

        if(seconds<0) seconds=0;
        if(seconds<=MaxSeconds)
        {
            unsigned       s = Min(static_cast<unsigned>(floor(seconds+0.5)),MaxSeconds);
            const unsigned d = s/Day;  s -= d*Day;
            const unsigned h = s/Hour; s -= h*Hour;
            const unsigned m = s/Minute; s -= m*Minute;

            if(d<=0)
            {
                snprintf(fmt, sizeof(fmt), "   %02u:%02u:%02d", h, m, s);
            }
            else
            {
                snprintf(fmt, sizeof(fmt), "%2uD%02u:%02u:%02d", d, h, m, s);
            }
        }
        else
        {
            static const char tooMuch[Length+1]= "99D+.......";
            memcpy(fmt,tooMuch,sizeof(tooMuch));
        }
    }

    HRT:: HRT(const HRT &other) noexcept : fmt() { memcpy(fmt,other.fmt,sizeof(fmt)); }


    std::ostream & operator<<(std::ostream &os, const HRT &hrt)
    {
        return os << hrt.fmt;
    }
}
