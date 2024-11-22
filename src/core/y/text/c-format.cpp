#include "y/text/c-format.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace Yttrium
{

    void C_Format:: Buffer(char * const       buffer,
                           const size_t       buflen,
                           const char * const fmt,
                           void * const       ptr) noexcept
    {

        assert(buffer!=0);
        assert(buflen>0);
        assert(0!=ptr);

        memset(buffer,0,buflen);
        va_list &ap = *static_cast<va_list *>(ptr);
        (void) vsnprintf(buffer,buflen-1,fmt,ap);
    }

}

