
#include "y/text/human-readable.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

namespace Yttrium
{

    static const uint64_t _1 = 1;

    const HumanReadable:: Divider HumanReadable::Divide[] =
    {

        { ' ', _1 << 0  },
        { 'k', _1 << 10 },
        { 'M', _1 << 20 },
        { 'G', _1 << 30 },
        { 'T', _1 << 40 },
        { 'P', _1 << 50 },
        { 'E', _1 << 60 },
    };




    const size_t HumanReadable::Number  = sizeof(HumanReadable::Divide) / sizeof(HumanReadable::Divide[0]);


    HumanReadable:: HumanReadable(const uint64_t u) noexcept :
    quot(0),
    rem(0),
    sfx(0)
    {

        if(u)
        {
            unsigned &p = Coerce(sfx);
            p = Number-1;
            uint64_t v = Divide[p].factor;
            while(u<v) { v = Divide[--p].factor; }
            Coerce(quot) = static_cast<unsigned>(u/v);
            Coerce(rem)  = static_cast<unsigned>(u-v*quot);
            assert(quot<1024);
        }

    }

    HumanReadable:: HumanReadable(const HumanReadable &hr) noexcept :
    quot(hr.quot),
    rem(hr.rem),
    sfx(hr.sfx)
    {

    }


    HumanReadable:: ~HumanReadable() noexcept
    {
    }

    std::ostream & operator<<(std::ostream &os, const HumanReadable &hr)
    {
        // request: 4 + 1 + precision
        char buf[512];
        char fmt[32];
        memset(buf,0,sizeof(buf));
        memset(fmt,0,sizeof(fmt));

        snprintf(fmt, sizeof(fmt), "%%4u.%%0%uu%%c",2);
        const HumanReadable::Divider &hd = HumanReadable::Divide[hr.sfx];
        const unsigned                fp = floor(100.0*double(hr.rem)/hd.factor);
        snprintf(buf, sizeof(buf),fmt,hr.quot, fp, hd.suffix);
        os << buf;
        return os;
    }

}
