
#include "y/vfs/local-fs.hpp"
#include "y/system/exception.hpp"
#include <unistd.h>

namespace Yttrium
{
    const char * const LocalFS:: CallSign = "LocalFS";
    
    LocalFS:: ~LocalFS() noexcept
    {
    }

    LocalFS:: LocalFS() noexcept :
    Singleton<LocalFS>(),
    VFS()
    {
    }

    bool LocalFS:: TryRemove(const String &path)
    {
#if defined(Y_BSD)
        if( 0!=unlink( path() ) ) return false;
        return true;
#endif

        throw Specific::Exception(CallSign,"TryRemove not implemented");
    }

}
