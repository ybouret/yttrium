
#include "y/utest/run.hpp"
#include "y/vfs/local/fs.hpp"

using namespace Yttrium;

Y_UTEST(vfs_subdirs)
{
    VFS &fs = LocalFS::Instance();

    if(argc>1)
    {
        fs.createSubDirs(argv[1]);
    }


}
Y_UDONE()
