#include "y/system/error.hpp"
#include "y/type/utils.hpp"

#include <cstring>



namespace Yttrium
{

    namespace Libc
    {
        void FormatError(char        *buffer,
                         const size_t buflen,
                         const int    err) noexcept
        {
            assert(Good(buffer,buflen));
            if(buflen>1)
            {
                memset(buffer,0,buflen);
                const char  *errbuf = strerror(err);
                const size_t errlen = strlen(errbuf);
                memcpy(buffer,errbuf,Min(errlen,buflen-1));
            }
        }
    }
    

}

