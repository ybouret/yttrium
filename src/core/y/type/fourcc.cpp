
#include "y/type/fourcc.hpp"
#include "y/text/hexadecimal.hpp"
#include <cstring>
#include <cctype>
#include <cstdio>

namespace Yttrium
{
    
#if 0
    static bool isAuthorized(const char c) noexcept
    {
        static const char extra[] = "-_:";
        return 0 != strchr(extra,c);
    }
#endif

    const char * FourCC:: ToText(const uint32_t uuid) noexcept
    {
        static char data[128];
        memset(data,0,sizeof(data));

        for(int shift=0;shift<=24;shift+=8)
        {
            const uint8_t b = uint8_t(uuid>>shift);
            const char    c = b;
            //if( isalnum(c)|| isAuthorized(c) )
            if( isgraph(c) )
            {
                char buff[8];
                memset(buff,0,sizeof(buff));
                snprintf(buff,sizeof(buff), "%c", c);
                strcat(data,buff);
            }
            else
            {
                strcat(data,Hexadecimal::Text[b]);
            }
        }


        return data;
    }

}

