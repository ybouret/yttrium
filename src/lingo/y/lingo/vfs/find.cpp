

#include "y/lingo/vfs/find.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        void VirtualFileSystem:: _Find(VFS                  & vfs,
                                       VFS::Entries         & entries,
                                       const String         & dirName,
                                       Matching             & matches,
                                       const Matching::Method how,
                                       const VFS::Entry::Part part)
        {
            const char * const    p_id = VFS::Entry::PartText(part);
            AutoPtr<VFS::Scanner> scan = vfs.openDirectory(dirName);
            while(true)
            {
                AutoPtr<VFS::Entry> ep = scan->get();   if(ep.isEmpty()) break;
                const String        es = ep->pry(part);
                if(!matches(how,p_id,es)) continue;
                //std::cerr << " (+) " << ep << std::endl;
                entries.pushTail(ep.yield());
            }
        }

        void VirtualFileSystem:: _Find(VFS                  & fileSys,
                                       VFS::Entries         & entries,
                                       const char * const     dirName,
                                       Matching             & matches,
                                       const Matching::Method how,
                                       const VFS::Entry::Part part)
        {
            const String _(dirName);
            return _Find(fileSys,entries,_,matches,how,part);
        }

        void VirtualFileSystem:: Display(VFS &, const VFS::Entry &ep)
        {
            std::cerr << ep << std::endl;
        }

        void VirtualFileSystem:: TryRemove(VFS &fs, const VFS::Entry &ep)
        {
            if(!ep.isReg()) return;
            fs.tryRemoveFile(ep.path);
        }

    }
}

