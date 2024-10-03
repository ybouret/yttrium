#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"


#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/container/cxx/array.hpp"

#include "y/stream/proc/input.hpp"
#include "y/stream/libc/output.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#include <errno.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace Yttrium;

namespace Yttrium
{
    struct CurrentWorkingDirectory
    {
        static String Get();
    };

    String CurrentWorkingDirectory:: Get()
    {
#if defined(Y_BSD)
        Y_GIANT_LOCK();
        size_t buflen = 4;
        while(true)
        {
            CxxArray<char> buffer(buflen);
            buflen          = buffer.size();
            const char *cwd = getcwd( &buffer[1], buffer.size() );
            if(!cwd)
            {
                const int err = errno;
                switch(err)
                {
                    case ERANGE: buflen <<= 1; continue;
                    default:
                        break;
                }
                throw Libc::Exception(err, "getcwd()");
            }
            return String(cwd);
        }
#endif

#if defined(Y_WIN)
		Y_GIANT_LOCK();
		const DWORD buflen = ::GetCurrentDirectory(0, NULL);
		if (buflen <= 0) throw Win32::Exception(::GetLastError(), "GetCurrentDirectory");
		CxxArray<char> buffer(buflen);
		const DWORD result = ::GetCurrentDirectory(buflen,&buffer[1]);
		if (result != buflen-1) throw Win32::Exception(::GetLastError(), "GetCurrentDirectory lengths mismatch!");
		return String(&buffer[1], result);
#endif

		throw Specific::Exception("CurrentWorkingDirectory::Get()", "not implemented on %s", Y_PLATFORM);
    }


}


Y_UTEST(vfs_cwd)
{
    String cwd = CurrentWorkingDirectory::Get();
    std::cerr << "cwd=" << cwd << std::endl;
    {
        OutputFile fp(StdErr);
#if defined(Y_BSD)
        ProcInput::SendTo(fp, "ls");
#endif

#if defined(Y_WIN)
		ProcInput::SendTo(fp, "dir");
#endif
        
    }
}
Y_UDONE()

