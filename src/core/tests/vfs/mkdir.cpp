#include "y/utest/run.hpp"
#include "y/vfs/local/fs.hpp"

using namespace Yttrium;

Y_UTEST(vfs_mkdir)
{
    VFS &fs = LocalFS::Instance();

    if(argc>1)
    {
        (void) fs.tryEraseEmpty(argv[1]);
        fs.makeDirectory(argv[1],false);
        fs.makeDirectory(argv[1],true);
        Y_CHECK(fs.tryEraseEmpty(argv[1]));;
    }
    

}
Y_UDONE()
