#include "y/vfs/vfs.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(vfs_names)
{
    for(int i=1;i<argc;++i)
    {
        const char * const path     = argv[i];
        const char *       baseName = VFS::BaseName(path);
        const char *       fileExt  = VFS::Extension(path);

        std::cerr << "[" << path << "]";
        std::cerr << " base=[" << baseName << "]";
        if(fileExt) std::cerr << " fileExt=[" << fileExt << "]";
        else        std::cerr << " NoExt";
        std::cerr << std::endl;

    }
}
Y_UDONE()

