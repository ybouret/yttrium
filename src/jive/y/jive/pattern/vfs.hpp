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
            static VFS & Find(VFS::Entries &         entries,
                              VFS &                  fileSys,
                              const String &         dirName,
                              Matcher &              rxMatch,
                              const VFS::Entry::Part thePart);

            //! alias
            static VFS & Find(VFS::Entries &         entries,
                              VFS &                  fileSys,
                              const char * const     dirName,
                              Matcher &              rxMatch,
                              const VFS::Entry::Part thePart);

            //! helper
            template <typename DIRNAME, typename REGEXPR> static inline
            VFS &List(VFS::Entries &         entries,
                      VFS &                  fileSys,
                      const DIRNAME &        dirName,
                      const REGEXPR &        regExpr,
                      const VFS::Entry::Part thePart)
            {
                Matcher matching(regExpr);
                return Find(entries,fileSys,dirName,matching,thePart);
            }
            
            template <typename DIRNAME, typename REGEXPR> static inline
            VFS &TryRemove(VFS &                  fileSys,
                           const DIRNAME &        dirName,
                           const REGEXPR &        regExpr,
                           const VFS::Entry::Part thePart)
            {
                VFS::Entries entries;
                VFS &fs = List(entries,fileSys,dirName,regExpr,thePart);
                while(entries.size)
                {
                    fs.tryRemoveFile(entries.tail->path);
                    delete entries.popTail();
                }
                return fs;
            }


        };
    }
}

#endif

