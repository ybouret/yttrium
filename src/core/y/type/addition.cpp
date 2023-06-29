#include "y/type/addition.hpp"
#include "y/system/exception.hpp"

#include <cerrno>

namespace Yttrium
{
    void Addition::RaiseOverflow(const char *ctx)
    {
        if(!ctx) ctx = "";
        throw Libc::Exception(ERANGE,"Overflow in addition %s",ctx);
    }
}
