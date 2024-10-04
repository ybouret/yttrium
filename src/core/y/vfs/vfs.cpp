
#include "y/vfs/vfs.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    VFS:: ~VFS() noexcept {}
    VFS::  VFS() noexcept {}

    bool VFS:: tryRemoveFile(const char *path)
    {
        const String _(path);
        return tryRemoveFile(_);
    }

}

namespace Yttrium
{
    const char VFS::EmptyName[] = "";

    bool VFS:: IsSeparator(const char c) noexcept
    {
        if(c=='/')  return true;
        if(c=='\\') return true;
        return false;
    }


    const char * VFS:: BaseName(const char * const path, const size_t size) noexcept
    {
        assert(Good(path,size));
        if(path==0)
        {
            return EmptyName;
        }
        else
        {
            const char *guess = path+size;
            const char *probe = guess-1;
            for(size_t i=size;i>0;--i,--guess,--probe)
            {
                if(IsSeparator(*probe)) return guess;
            }
            return path;
        }
    }

    const char * VFS:: BaseName(const char * const path) noexcept
    {
        return BaseName(path,StringLength(path));
    }

    const char * VFS:: BaseName(const String & path) noexcept
    {
        return BaseName(path(),path.size());
    }
}

namespace Yttrium
{
    const char * VFS:: Extension(const char * const path, const size_t size) noexcept
    {
        assert(Good(path,size));
        const char * const base = BaseName(path,size);
        if(!base) return 0;
        const size_t blen = size - static_cast<size_t>(base-path);
        assert(StringLength(base) == blen);

        const char *dot = base+blen;
        while(--dot>=base)
        {
            if('.'==*dot) return dot;
        }

        return 0;
    }

    const char * VFS:: Extension(const char * const path) noexcept
    {
        return Extension(path,StringLength(path));
    }

    const char * VFS:: Extension(const String & path) noexcept
    {
        return Extension(path(),path.size());
    }
}


namespace Yttrium
{
    String VFS:: ChangedExtension(const char * const newExt,
                                  const size_t       extLen,
                                  const char * const path,
                                  const size_t       size)
    {
        assert(Good(newExt,extLen));
        assert(Good(path,size));
        const char *ext = Extension(path,size);
        if(!ext)
        {
            String res(path,size);
            res << '.';
            res.pushAtTail(newExt,extLen);
            return res;
        }
        else
        {
            String res(path,1+ext-path);
            res.pushAtTail(newExt,extLen);
            return res;
        }
    }

    String VFS:: ChangedExtension(const char * const  newExt, const String & path)
    {
        return ChangedExtension(newExt, StringLength(newExt),path(),path.size());
    }

    String VFS:: ChangedExtension(const String & newExt, const String & path)
    {
        return ChangedExtension(newExt(), newExt.size(),path(),path.size());
    }

}

namespace Yttrium
{
    String VFS:: RemovedExtension(const char * const path, const size_t size)
    {
        assert(Good(path,size));
        const char *ext = Extension(path,size);
        if(!ext)
        {
            return String(path,size);
        }
        else
        {
            return String(path,ext-path);
        }
    }

    String VFS:: RemovedExtension(const String& path)
    {
        return RemovedExtension(path.c_str(), path.size());
    }

}


namespace Yttrium
{


    String VFS:: MakeDirName(const char *const path, const size_t size)
    {
        assert(Good(path,size));
        if(size<=0)
        {
            return String("./");
        }
        else
        {

            String dirName(size,AsCapacity,false);
            const char *       curr = path;
            const char * const last = path + size;

            while(curr<last)
            {
                if(IsSeparator(*curr))
                {
                    // skip successive separators
                    while(++curr<last && IsSeparator(*curr))
                        ;
                    dirName += '/';
                }
                else
                {
                    // save successive not-sep
                    do
                    {
                        dirName += *curr;
                    } while(++curr<last && !IsSeparator(*curr) );
                }
            }

            if(dirName.size()<=0 || !IsSeparator(dirName.tail()) )
                dirName += '/';

            return dirName;
        }
    }

    String VFS:: MakeDirName(const char *const path)
    {
        return MakeDirName(path,StringLength(path));
    }

    String VFS:: MakeDirName(const String &path)
    {
        return MakeDirName(path.c_str(),path.size());
    }

}

namespace Yttrium
{

    VFS::Scanner * VFS:: openDirectory(const char *dirName)
    {
        const String _(dirName);
        return openDirectory(_);
    }

    VFS::EntryType VFS:: findEntryType(const char   *path, bool &link) const
    {
        const String _(path);
        return findEntryType(_,link);
    }

    const char * VFS:: EntryTypeText(const EntryType t) noexcept
    {
        switch(t)
        {
            case IsReg: return "f";
            case IsDir: return "d";
            case IsUnk: return "?";
        }
        return "!";
    }

    int VFS:: EntryTypeChar(const EntryType t) noexcept
    {
        switch(t)
        {
            case IsReg: return 'f';
            case IsDir: return 'd';
            case IsUnk: break;;
        }
        return 'u';
    }


    
    void VFS:: makeDirectory(const char *dirName, const bool mayExist)
    {
        const String _(dirName);
        makeDirectory(_,mayExist);
    }

    bool VFS:: tryEraseEmpty(const char * const dirName)
    {
        const String _(dirName);
        return tryEraseEmpty(_);
    }


}

namespace Yttrium
{
    void VFS:: setCWD(const char * const dirName)
    {
        const String _(dirName);
        setCWD(_);
    }

}


#include "y/ptr/auto.hpp"

namespace Yttrium
{
    void VFS:: addTo(Entries &entries, const String &dirName, const AddMode mode)
    {
        AutoPtr<Scanner> scan = openDirectory(dirName);
        while( true )
        {
            AutoPtr<VFS::Entry> ep = scan->get();
            if(ep.isEmpty()) return;

            if(ep->isDot())  continue;
            if(ep->isDDot()) continue;
            switch(ep->type)
            {
                case IsUnk:
                    continue;

                case IsReg:
                    if(mode==AddDir) continue;
                    break;

                case IsDir:
                    if(mode==AddReg) continue;
                    break;
            }

            entries.pushTail(ep.yield());
        }
    }

    void VFS:: addTo(Entries &entries, const char * const dirName, const AddMode mode)
    {
        const String _(dirName);
        addTo(entries, _, mode);
    }


}
