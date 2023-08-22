
#include "y/vfs/vfs.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    VFS:: ~VFS() noexcept {}
    VFS::  VFS() noexcept {}

    bool VFS:: TryRemove(const char   *path)
    {
        const String _(path);
        return TryRemove(_);
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
    String VFS:: ChangeExtension(const char * const newExt,
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

    String VFS:: ChangeExtension(const char * const  newExt, const String & path)
    {
        return ChangeExtension(newExt, StringLength(newExt),path(),path.size());
    }

    String VFS:: ChangeExtension(const String & newExt, const String & path)
    {
        return ChangeExtension(newExt(), newExt.size(),path(),path.size());
    }

}


namespace Yttrium
{

    VFS::Scanner * VFS:: OpenDirectory(const char *dirName)
    {
        const String _(dirName);
        return OpenDirectory(_);
    }

}
