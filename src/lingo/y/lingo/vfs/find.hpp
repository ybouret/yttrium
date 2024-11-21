//! \file

#ifndef Y_Lingo_VFS_Find_Included
#define Y_Lingo_VFS_Find_Included 1

#include "y/lingo/pattern/matching.hpp"
#include "y/vfs/vfs.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! VFS functions
        //
        //
        //______________________________________________________________________
        struct VirtualFileSystem
        {
            //__________________________________________________________________
            //
            //
            //! append entries
            //
            //______________________________________________________________________
            static   void _Find(VFS                   &fileSys,
                                VFS::Entries          &entries,
                                const String          &dirName,
                                Matching              &matches,
                                const Matching::Method how,
                                const VFS::Entry::Part part);
            //__________________________________________________________________
            //
            //
            //! append entries
            //
            //__________________________________________________________________
            static   void _Find(VFS                &   fileSys,
                                VFS::Entries       &   entries,
                                const char * const     dirName,
                                Matching           &   matches,
                                const Matching::Method how,
                                const VFS::Entry::Part part);

            //__________________________________________________________________
            //
            //
            //! generic call
            //
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //
            //! apply procedure to each matching entry
            //
            //__________________________________________________________________
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

            static void Display(VFS &, const VFS::Entry &ep);      //!< mostly to debug
            static void TryRemove(VFS &fs, const VFS::Entry &ep);  //!< try remove if regular files


        };
    }
}

#endif

