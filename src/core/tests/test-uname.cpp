
#include "y/utest/run.hpp"

#if defined(Y_BSD)
#include <sys/utsname.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace Yttrium;

#define SHOW_FIELD(NAME) std::cerr << "\t"<<  #NAME << " : " << buf.NAME << std::endl

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
    
#if defined(Y_WIN)
	SYSTEM_INFO buf;
	std::cerr << "GetSystemInfo:" << std::endl;
	::GetSystemInfo(&buf);
	SHOW_FIELD(dwNumberOfProcessors);
	SHOW_FIELD(dwPageSize);
	SHOW_FIELD(dwProcessorType);
	SHOW_FIELD(wProcessorArchitecture);
#if 0
	std::cerr << "GetNativeSystemInfo:" << std::endl;
	::GetNativeSystemInfo(&buf);
	SHOW_FIELD(dwNumberOfProcessors);
	SHOW_FIELD(dwPageSize);
	SHOW_FIELD(dwProcessorType);
	SHOW_FIELD(wProcessorArchitecture);
#endif
#endif
}
Y_UDONE()

