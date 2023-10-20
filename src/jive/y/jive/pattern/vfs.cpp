
#include "y/jive/pattern/vfs.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Jive
    {
        void VirtualFileSystem:: Find(VFS          &vfs,
                                      const String &dname,
                                      VFS::Entries &elist,
                                      Matcher      &match)
        {
            elist.release();
            const String          path = VFS::MakeDirName(dname);
            AutoPtr<VFS::Scanner> scan = vfs.openDirectory(path);
            AutoPtr<VFS::Entry>   ep   = 0;
            while( (ep = scan->get() ).isValid() )
            {
                if(!ep->isReg()) continue;
                if( match.exactly(ep->path,ep->base) )
                {
                    elist.pushTail( ep.yield() );
                }
            }
            MergeSort::Call(elist,VFS::Entry::CompareByName);
        }

        void VirtualFileSystem:: Find(VFS          &       vfs,
                                      const char   * const dname,
                                      VFS::Entries &       elist,
                                      Matcher      &       match)
        {
            const String _(dname);
            Find(vfs,_,elist,match);
        }
    }
}
