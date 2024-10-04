
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
            std::cerr << "Raw Listing" << std::endl;
            AutoPtr<VFS::Scanner> scanner = fs.openDirectory(argv[1]);
            VFS::Entry *ep;
            while( 0 != (ep=scanner->get()))
            {
                entries.pushTail(ep);
                std::cerr << "\t" << ep->path << ", " << ep->base << std::endl;
                if(ep->isDot()) std::cerr << "\t|_[DOT]" << std::endl;
                if(ep->isDDot()) std::cerr << "\t|_[DDOT]" << std::endl;

            }
        }
        entries.release();

        std::cerr << std::endl;
        std::cerr << "addReg" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddReg);
        std::cerr << entries << std::endl;

        entries.release();
        std::cerr << std::endl;
        std::cerr << "addDir" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddDir);
        std::cerr << entries << std::endl;

        entries.release();
        std::cerr << std::endl;
        std::cerr << "addAny" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddAny);
        std::cerr << entries << std::endl;

    }

}
Y_UDONE()
