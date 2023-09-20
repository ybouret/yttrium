
#include "y/vfs/local-fs.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;

Y_UTEST(vfs_entry)
{
    VFS &fs = LocalFS::Instance();

    for(int i=1;i<argc;++i)
    {
        const char * const  path     = argv[i];
        AutoPtr<VFS::Entry> ep = new VFS::Entry(fs,path);
        const VFS::Entry    cp = *ep;
        std::cerr << ep << std::endl;
        std::cerr << cp << std::endl;

    }
}
Y_UDONE()

