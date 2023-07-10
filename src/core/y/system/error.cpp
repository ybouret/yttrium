#include "y/system/error.hpp"
#include "y/type/utils.hpp"
#include "y/text/ops.hpp"

#include <cstring>
#include <cstdarg>
#include <cstdio>



namespace Yttrium
{

    static inline void CleanError(char *output) noexcept
    {
        assert(output);
        static const char   bad[] = { ' ', '\t', '\r', '\n' };
        static const size_t num   = Y_STATIC_SIZE(bad);

        (void) TextOps::TrimInvalid(output,strlen(output),bad,num);
    }

    namespace Libc
    {
        void FormatError(char*        buffer,
                         const size_t buflen,
                         const int    err) noexcept
        {
            assert(Good(buffer, buflen));
            if(buffer)
            {
                CleanError(TextOps::CopyMessage(buffer,buflen,strerror(err)));
            }
        }

        void CriticalError(const int err, const char* fmt, ...)
        {
            static const char pfx[] = " *** ";
            assert(NULL != fmt);

            fputs(pfx, stderr);
            {
                va_list ap;
                va_start(ap, fmt);
                vfprintf(stderr, fmt, ap);
                va_end(ap);
            }
            fputc('\n', stderr);

            fputs(pfx, stderr);
            char buffer[256];
            Libc::FormatError(buffer, sizeof(buffer), err);
            fputs(buffer, stderr);
            fputc('\n', stderr);

            abort();
        }
    }


}

#if defined(Y_WIN)
#include <windows.h>

namespace Yttrium
{
    namespace Win32
    {
        //______________________________________________________________________
        //
        //
        //! using strerror
        //
        //______________________________________________________________________
        void FormatError(char  *        buffer,
                         const size_t   buflen,
                         const uint32_t err) noexcept
        {
            assert(Good(buffer, buflen));
            memset(buffer, 0, buflen);
            if (buflen > 0)
            {
                const DWORD dw = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                               NULL,  /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
                                               err,
                                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                               buffer,
                                               (DWORD)buflen,
                                               NULL);
                if(dw <= 0)
                {
                    TextOps::CopyMessage(buffer, buflen, Core::Failure);
                }
                else
                {
                    CleanError(buffer);
                }
            }
        }

        void CriticalError(const uint32_t err, const char* fmt, ...)
        {
            static const char pfx[] = " *** ";
            assert(NULL != fmt);

            fputs(pfx, stderr);
            {
                va_list ap;
                va_start(ap, fmt);
                vfprintf(stderr, fmt, ap);
                va_end(ap);
            }
            fputc('\n', stderr);

            fputs(pfx, stderr);
            char buffer[256];
            Win32::FormatError(buffer, sizeof(buffer), err);
            fputs(buffer, stderr);
            fputc('\n', stderr);

            abort();
        }
    }

}

#endif
