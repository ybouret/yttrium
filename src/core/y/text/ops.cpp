
#include "y/text/ops.hpp"
#include "y/type/utils.hpp"
#include <cstring>
#include <cctype>

namespace Yttrium
{

    char * TextOps:: CopyMessage(char *buffer, const size_t buflen, const char *msg) noexcept
    {
        assert( Good(buffer,buflen) );
        memset(buffer,0,buflen);
        if(msg)
        {
            const size_t msglen = strlen(msg);
            memcpy(buffer,msg, Min(msglen,buflen-1) );
        }
        return buffer;
    }


    static inline bool isBad(const int C, const char *bad, const size_t num) noexcept
    {
        assert( Good(bad,num) );
        for(size_t i=0;i<num;++i) {
            if( C == bad[i] ) return true;
        }
        return false;
    }

    char * TextOps:: TrimInvalid(char *buffer, const size_t buflen, const char *bad, const size_t num) noexcept
    {
        assert( Good(buffer,buflen) );
        assert( Good(bad,num) );

        if(buflen>0&&num>0)
        {
            char *end = buffer+buflen;
            while(--end>=buffer)
            {
                if(isBad(*end,bad,num))
                {
                    *end=0;
                    continue;
                }
                break;
            }
        }
        return buffer;
    }

    void  TextOps:: ToLower(char *buffer, const size_t buflen) noexcept
    {
        assert( Good(buffer,buflen) );
        for(size_t i=0;i<buflen;++i) buffer[i] = char(tolower(buffer[i]));
    }


}

