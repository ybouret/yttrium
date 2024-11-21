

#include "y/lingo/vfs/find.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"
#include "y/vfs/local/fs.hpp"

using namespace Yttrium;
using namespace Lingo;


static inline
void doFind(VFS &            fs,
            const String    &dirName,
            Matching        &matches,
            Matching::Method how,
            VFS::Entry::Part part)
{
    VFS::Entries entries;
    std::cerr << "\tmatching " << VFS::Entry::PartText(part) << std::endl;
    VirtualFileSystem::_Find(fs, entries, dirName, matches, how, part);
    for(const VFS::Entry *ep=entries.head;ep;ep=ep->next)
    {
        std::cerr << "\t\t" << *ep << std::endl;
    }
}


static inline void doFindAll(VFS &             fs,
                             const String     &dirName,
                             Matching         &matches,
                             Matching::Method how)
{
    doFind(fs,dirName,matches,how,VFS::Entry::Path);
    doFind(fs,dirName,matches,how,VFS::Entry::Base);
    doFind(fs,dirName,matches,how,VFS::Entry::PathWE);
    doFind(fs,dirName,matches,how,VFS::Entry::BaseWE);
    doFind(fs,dirName,matches,how,VFS::Entry::Ext);
}


Y_UTEST(vfs)
{
    VFS &fs = LocalFS::Instance();
    if(argc>1)
    {
        Matching     matches = argv[1];
        if(argc>2)
        {

            const String dirName = argv[2];
            std::cerr << "Matching::Exactly" << std::endl;
            doFindAll(fs, dirName, matches, Matching::Exactly);

            std::cerr << std::endl;
            std::cerr << "Matching::Somehow" << std::endl;
            doFindAll(fs, dirName, matches, Matching::Somehow);

            std::cerr << std::endl;
            std::cerr << "With extension:" << std::endl;
            VirtualFileSystem::ForEach(fs,argv[2], argv[1], Matching::Exactly, VFS::Entry::Ext, VirtualFileSystem::Display);
        }

    }
}
Y_UDONE()
