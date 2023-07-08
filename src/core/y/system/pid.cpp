
#include "y/system/pid.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#endif

#if defined(Y_WIN)
#include <windows.h>
#endif

namespace Yttrium
{
    uint64_t ProcessId:: Get() noexcept
    {
#if defined(Y_BSD)
        union
        {
            uint64_t q;
            pid_t    p;

        } alias = { 0 };
        alias.p = getpid();
        return alias.q;
#endif

#if defined(Y_WIN)
        return ::GetCurrentProcessId();
#endif
    }
}

