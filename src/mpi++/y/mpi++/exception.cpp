
#include "y/mpi++/api.hpp"
#include <cstring>
#include <cstdarg>

namespace Yttrium
{

    MPI::Exception:: ~Exception() noexcept {}

    MPI::Exception:: Exception(const Exception &excp) noexcept :
    Yttrium::Exception(excp), code(excp.code), mesg()
    {
        memcpy(mesg,excp.mesg,sizeof(mesg));
    }

    MPI::Exception:: Exception(const int   err,
                               const char *fmt,
                               ...) noexcept :
    Yttrium::Exception(), code(err), mesg()
    {
        memset(mesg,0,sizeof(mesg));
        {
            assert(0!=fmt);
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
        }
        int mlen = 0;
        MPI_Error_string(code,mesg,&mlen);
    }

    const char * MPI::Exception:: what() const noexcept
    {
        return mesg;
    }

}
