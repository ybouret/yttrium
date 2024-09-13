
#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;

Y_UTEST(vfs_scan)
{
    VFS &fs = LocalFS::Instance();

    if(argc>1)
    {
        VFS::Entries entries;
        {
            AutoPtr<VFS::Scanner> scanner = fs.openDirectory(argv[1]);
            VFS::Entry *ep;
            while( 0 != (ep=scanner->get()))
            {
                entries.pushTail(ep);
                std::cerr << ep->path << ", " << ep->base << std::endl;
            }
        }
    }

}
Y_UDONE()
