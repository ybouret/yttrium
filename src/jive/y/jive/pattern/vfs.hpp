//! \file

#ifndef Y_Jive_Pattern_VFS_Included
#define Y_Jive_Pattern_VFS_Included 1

#include "y/jive/pattern/matcher.hpp"
#include "y/vfs/vfs.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Operations on VFS
        //
        //
        //______________________________________________________________________
        struct VirtualFileSystem
        {
            //! find in VFS directory the list of matching regular entries
            static void Find(VFS          &vfs,
                             const String &dname,
                             VFS::Entries &elist,
                             Matcher      &match);

            //! alias
            static void Find(VFS          &       vfs,
                             const char   * const dname,
                             VFS::Entries &       elist,
                             Matcher      &       match);
        };
    }
}

#endif

