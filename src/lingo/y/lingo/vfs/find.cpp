

#include "y/lingo/vfs/find.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        void VirtualFileSystem:: Find(VFS                &vfs,
                                      VFS::Entries       &entries,
                                      const String       &dirName,
                                      Matching           &matches,
                                      Matching::Method    how,
                                      VFS::Entry::Part    part)
        {
            const char * const    p_id = VFS::Entry::PartText(part);
            AutoPtr<VFS::Scanner> scan = vfs.openDirectory(dirName);
            while(true)
            {
                AutoPtr<VFS::Entry> ep = scan->get();   if(ep.isEmpty()) break;
                const String        es = ep->pry(part);
                if(!matches(how,p_id,es)) continue;
                entries.pushTail(ep.yield());
            }

        }
    }
}

