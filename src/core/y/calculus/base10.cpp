
#include "y/calculus/base10.hpp"
#include <cstring>
#include <cstdio>

namespace Yttrium
{
    unsigned Base10:: DigitsForU64(uint64_t u) noexcept
    {

        unsigned res = 1;
        while(true)
        {
            const uint64_t quot = u / 10;
            if(quot<=0) break;
            ++res;
            u = quot;
        }

        return res;
    }

    Base10:: Format:: Format(const uint64_t u) noexcept :
    buffer()
    {
        memset(buffer,0,sizeof(buffer));
        const unsigned n = DigitsForU64(u);
        //std::cerr << n << " digits for  " << u << std::endl;
        snprintf(buffer, sizeof(buffer)-1, "%%0%uu", n);
    }

    Base10:: Format:: ~Format() noexcept
    {
        memset(buffer,0,sizeof(buffer));
    }

    Base10:: Format:: Format(const Format &fmt) noexcept :
    buffer()
    {
        memcpy(buffer,fmt.buffer,sizeof(buffer));
    }

    const char * Base10:: Format:: operator*() const noexcept
    {
        return buffer;
    }

}

