
#include "y/utest/run.hpp"

#if defined(Y_BSD)
#include <sys/utsname.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace Yttrium;

#define SHOW_FIELD(NAME) std::cerr << #NAME << " : " << buf.NAME << std::endl

Y_UTEST(uname)
{
#if defined(Y_BSD)
    struct utsname buf;
    if( 0!= uname( &buf ) )
    {
        std::cerr << "Error" << std::endl;
    }
    else
    {
        SHOW_FIELD(machine);
        SHOW_FIELD(sysname);
        SHOW_FIELD(release);
        SHOW_FIELD(version);
        SHOW_FIELD(nodename);
    }
#endif
    
}
Y_UDONE()

