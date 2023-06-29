#include "y/system/error.hpp"
#include "y/type/utils.hpp"

#include <cstring>
#include <cstdarg>
#include <cstdio>



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

        void CriticalError(const int err, const char *fmt,...)
        {
            static const char pfx[] = " *** ";
            assert(NULL!=fmt);

            fputs(pfx,stderr);
            {
                va_list ap;
                va_start(ap,fmt);
                vfprintf(stderr,fmt,ap);
                va_end(ap);
            }
            fputc('\n',stderr);

            fputs(pfx,stderr);
            char buffer[256];
            Libc:: FormatError(buffer, sizeof(buffer), err);
            fputs(buffer,stderr);
            fputc('\n',stderr);

            abort();
        }
    }
    

}

