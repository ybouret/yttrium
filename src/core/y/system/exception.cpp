#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/type/utils.hpp"
#include <cstdarg>
#include <cstring>

namespace Yttrium
{

    namespace Specific
    {

        Exception:: ~Exception() noexcept {}

        Exception:: Exception(const char *header,
                              const char *fmt,...) noexcept :
        Yttrium::Exception(),
        title()
        {
            memset(title,0,sizeof(title));
            memcpy(title,header,Min(Core::Length(header),sizeof(title)-1));
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
        }

        Exception:: Exception(const Exception &excp) noexcept :
        Yttrium::Exception(excp),
        title()
        {
            memcpy(title,excp.title,sizeof(title));
        }

        const char *Exception:: what() const noexcept
        {
            return title;
        }

    }

    namespace Libc
    {

        Exception:: ~Exception() noexcept {}

        Exception:: Exception(const int   err,
                              const char *fmt,
                              ...) noexcept :
        Yttrium::Exception(),
        code(err),
        title()
        {
            Libc::FormatError(title,sizeof(title),err);
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
        }

        Exception:: Exception(const Exception &excp) noexcept :
        Yttrium::Exception(excp),
        code(excp.code),
        title()
        {
            memcpy(title,excp.title,sizeof(title));
        }

        const char *Exception:: what() const noexcept
        {
            return title;
        }

    }

#if defined(Y_WIN)
    namespace Win32
    {

        Exception:: ~Exception() noexcept {}

        Exception::Exception(const uint32_t   err,
                             const char* fmt,
                             ...) noexcept :
        Yttrium::Exception(),
        code(err),
        title()
        {
            Win32::FormatError(title, sizeof(title), err);
            va_list ap;
            va_start(ap, fmt);
            format(fmt, &ap);
            va_end(ap);
        }

        Exception::Exception(const Exception& excp) noexcept :
        Yttrium::Exception(excp),
        code(excp.code),
        title()
        {
            memcpy(title, excp.title, sizeof(title));
        }

        const char* Exception::what() const noexcept
        {
            return title;
        }

    }
#endif

}

#if defined(Y_Darwin)
#include <mach/mach.h>
#include "y/text/ops.hpp"

namespace Yttrium
{

    namespace Mach
    {

        Exception:: ~Exception() noexcept {}

        Exception:: Exception(const int   err,
                              const char *fmt,
                              ...) noexcept :
        Yttrium::Exception(),
        code(err),
        title()
        {
            TextOps::CopyMessage(title,sizeof(title),mach_error_string(err));
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
        }

        Exception:: Exception(const Exception &excp) noexcept :
        Yttrium::Exception(excp),
        code(excp.code),
        title()
        {
            memcpy(title,excp.title,sizeof(title));
        }

        const char *Exception:: what() const noexcept
        {
            return title;
        }

    }

}


#endif
