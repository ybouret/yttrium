#include "y/vfs/local/fs.hpp"

#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/container/cxx/array.hpp"


#if defined(Y_BSD)
#include <unistd.h>
#include <errno.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
namespace Yttrium
{

    String    LocalFS:: getCWD()
    {
        static const char fn[] = "LocalFS::GetCWD()";

#if defined(Y_BSD)
        Y_GIANT_LOCK();
        size_t buflen = 200;
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
                throw Libc::Exception(err,fn);
            }
            return String(cwd);
        }
#endif

#if defined(Y_WIN)
        Y_GIANT_LOCK();
        const DWORD buflen = ::GetCurrentDirectory(0, NULL);
        if (buflen <= 0) throw Win32::Exception(::GetLastError(), fn);
        CxxArray<char> buffer(buflen);
        const DWORD result = ::GetCurrentDirectory(buflen,&buffer[1]);
        if (result != buflen-1)
        {
            const DOWRD err = ::GetLastError();
            throw Win32::Exception(err, "%s lengths mismatch!", fn);
        }
        return String(&buffer[1], result);
#endif

        throw Specific::Exception(fn, "not implemented on %s", Y_PLATFORM);
    }

    void LocalFS:: setCWD(const String &dirName)
    {
#if defined(Y_BSD)
        Y_GIANT_LOCK();
        const char * const path = dirName.c_str();
        if( 0 !=  chdir( path ) )
        {
            const int err = errno;
            throw Libc::Exception(err,"chdir(%s)",path);
        }
#endif

#if defined(Y_WIN)
        Y_GIANT_LOCK();
        const char * const path = dirName.c_str();
        if( ! ::SetCurrentDirectory(path) )
        {
            const DWORD err = ::GetLastError();
            throw Win32::Exception(err, "SetCurrentDirectory(%s)", path);
        }
#endif

    }

}
