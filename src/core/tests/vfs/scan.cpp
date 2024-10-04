
#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/sort/merge.hpp"

using namespace Yttrium;


namespace {
    static inline void displayEntries(const VFS::Entries &entries)
    {
        for(const VFS::Entry *ep=entries.head;ep;ep=ep->next)
        {
            std::cerr << *ep << std::endl;
        }
    }
}
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
                std::cerr << "\t" << std::setw(32) << ep->path << ", " << ep->base << std::endl;
                if(ep->isDot()) std::cerr << "\t|_[DOT]" << std::endl;
                if(ep->isDDot()) std::cerr << "\t|_[DDOT]" << std::endl;

            }
        }
        displayEntries(entries);
        entries.release();

        std::cerr << std::endl;
        std::cerr << "addReg" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddReg);
        displayEntries(entries);

        entries.release();
        std::cerr << std::endl;
        std::cerr << "addDir" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddDir);
        displayEntries(entries);

        entries.release();
        std::cerr << std::endl;
        std::cerr << "addAny" << std::endl;
        fs.addTo(entries, argv[1], VFS::AddAny);
        displayEntries(entries);

        std::cerr << std::endl;
        std::cerr << "Compared by Path" << std::endl;
        MergeSort::Call(entries, VFS::Entry::CompareByPath );
        displayEntries(entries);
        std::cerr << std::endl;

        std::cerr << "Compared by Path, Dir First" << std::endl;
        MergeSort::Call(entries, VFS::Entry::CompareByPathDirFirst );
        displayEntries(entries);
        std::cerr << std::endl;

        std::cerr << "Compared by Name" << std::endl;
        MergeSort::Call(entries, VFS::Entry::CompareByName );
        displayEntries(entries);


    }

}
Y_UDONE()
