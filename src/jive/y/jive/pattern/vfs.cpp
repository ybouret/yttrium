
#include "y/jive/pattern/vfs.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Jive
    {
        VFS & VirtualFileSystem:: Find(VFS::Entries &         entries,
                                       VFS &                  fileSys,
                                       const String &         dirName,
                                       Matcher &              rxMatch,
                                       const VFS::Entry::Part thePart)
        {
            entries.release();
            const String          path = VFS::MakeDirName(dirName);
            AutoPtr<VFS::Scanner> scan = fileSys.openDirectory(path);
            AutoPtr<VFS::Entry>   ep   = 0;
            while( (ep = scan->get() ).isValid() )
            {
                if(!ep->isReg()) continue;
                const String data = ep->pry(thePart);
                if( rxMatch.exactly(ep->path,data) )
                {
                    entries.pushTail( ep.yield() );
                }
            }
            MergeSort::Call(entries,VFS::Entry::CompareByName);
            return fileSys;
        }

        VFS & VirtualFileSystem:: Find(VFS::Entries &         entries,
                                       VFS &                  fileSys,
                                       const char * const     dirName,
                                       Matcher &              rxMatch,
                                       const VFS::Entry::Part thePart)
        {
            const String _(dirName);
            return Find(entries,fileSys,_,rxMatch,thePart);
        }
    }
}
