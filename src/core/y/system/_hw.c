
#include "y/config/starting.h"

#if defined(Y_Darwin)
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(Y_Linux) || defined(Y_FreeBSD) || defined(Y_SunOS) || defined(Y_OpenBSD)
#include <unistd.h>
#endif

size_t Yttrium_Hardware_NumProcs()
{

#if defined(Y_Darwin)
    int      nm[2] = { CTL_HW, HW_AVAILCPU };
    size_t   len = 4;
    uint32_t count=0;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#endif
    
#if defined(Y_WIN)
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    const DWORD n = sysinfo.dwNumberOfProcessors;
    return n <= 1 ? 1:n;
#endif

#if defined(Y_Linux) || defined(Y_FreeBSD) || defined(Y_SunOS) || defined(Y_OpenBSD)
    const long n = sysconf( _SC_NPROCESSORS_ONLN );
    return n <= 0  ? 1 : n;
#endif


}
