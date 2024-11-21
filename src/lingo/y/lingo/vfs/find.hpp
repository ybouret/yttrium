//! \file

#ifndef Y_Lingo_VFS_Find_Included
#define Y_Lingo_VFS_Find_Included 1

#include "y/lingo/pattern/matching.hpp"
#include "y/vfs/vfs.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        struct VirtualFileSystem
        {
            //! append entries
            static  
            void Find(VFS                &vfs,
                      VFS::Entries       &entries,
                      const String       &dirName,
                      Matching           &matches,
                      Matching::Method    how,
                      VFS::Entry::Part    part);

        };
    }
}

#endif

