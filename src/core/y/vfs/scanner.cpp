
#include "y/vfs/vfs.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    VFS::Scanner:: Scanner(const VFS &_, const String &__)  :
    fs(_), directory(__)
    {
        String tmp = MakeDirName(directory);
        Coerce(directory).swapWith(tmp);
    }

    VFS::Scanner:: ~Scanner() noexcept
    {
    }

    
    
}
