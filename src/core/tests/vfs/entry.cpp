
#include "y/vfs/local/fs.hpp"
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
#define SHOW_AS(ID) std::cerr << std::setw(32) << #ID << " = " << ep->pry(ID) << std::endl

        SHOW_AS(VFS::Entry::Path);
        SHOW_AS(VFS::Entry::PathWE);
        SHOW_AS(VFS::Entry::Base);
        SHOW_AS(VFS::Entry::BaseWE);
        SHOW_AS(VFS::Entry::Ext);

    }
}
Y_UDONE()

