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
        code(0),
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

}


