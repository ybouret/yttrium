
#include "y/vfs/vfs.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    VFS::Scanner:: Scanner(const VFS &_, const String &__)  :
    vfs(_), dir(__)
    {
        String tmp = MakeDirName(dir);
        Coerce(dir).swapWith(tmp);
    }

    VFS::Scanner:: ~Scanner() noexcept
    {
    }

    
    
}
