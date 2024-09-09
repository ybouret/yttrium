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
            //! find in VFS directory the list of matching REGULAR entries
            static void Find(VFS &                  vfs,
                             const String &         dname,
                             VFS::Entries &         elist,
                             Matcher &              match,
                             const VFS::Entry::Part part);

            //! alias
            static void Find(VFS &                  fs,
                             const char * const     name,
                             VFS::Entries &         elist,
                             Matcher      &         match,
                             const VFS::Entry::Part part);

            //! helper
            template <typename DIRNAME, typename REGEXPR> static inline
            VFS &List(VFS::Entries &         entries,
                      VFS &                  fileSys,
                      const DIRNAME &        dirName,
                      const REGEXPR &        regExpr,
                      const VFS::Entry::Part thePart)
            {
                Matcher matching(regExpr);
                Find(fileSys,dirName,entries,matching,thePart);
                return fileSys;
            }

        };
    }
}

#endif

