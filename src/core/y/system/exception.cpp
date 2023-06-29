#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include <cstdarg>
#include <cstring>

namespace Yttrium
{

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


