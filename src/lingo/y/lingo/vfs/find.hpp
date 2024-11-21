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
            static   void _Find(VFS                   &fileSys,
                                VFS::Entries          &entries,
                                const String          &dirName,
                                Matching              &matches,
                                const Matching::Method how,
                                const VFS::Entry::Part part);

            //! append entries
            static   void _Find(VFS                &   fileSys,
                                VFS::Entries       &   entries,
                                const char * const     dirName,
                                Matching           &   matches,
                                const Matching::Method how,
                                const VFS::Entry::Part part);

            template <
            typename DIRECTORY,
            typename EXPRESSION
            > static inline
            void Find(VFS                   &fileSys,
                      VFS::Entries          &entries,
                      const DIRECTORY       &dirName,
                      const EXPRESSION      &regExpr,
                      const Matching::Method how,
                      const VFS::Entry::Part part)
            {
                Matching matches(regExpr);
                return _Find(fileSys,entries,dirName,matches,how,part);
            }

            static void Display(VFS &, const VFS::Entry &ep);
            static void TryRemove(VFS &fs, const VFS::Entry &ep);

            

            template <
            typename DIRECTORY,
            typename EXPRESSION,
            typename PROCEDURE
            > static inline
            void ForEach(VFS                  & fileSys,
                         const DIRECTORY      & dirName,
                         const EXPRESSION     & regExpr,
                         const Matching::Method how,
                         const VFS::Entry::Part part,
                         PROCEDURE             &proc)
            {
                VFS::Entries entries;
                Find(fileSys,entries,dirName,regExpr,how,part);
                for(const VFS::Entry *ep=entries.head;ep;ep=ep->next)
                    proc(fileSys,*ep);

            }



        };
    }
}

#endif

