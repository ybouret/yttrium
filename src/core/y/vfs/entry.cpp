#include "y/vfs/vfs.hpp"

namespace Yttrium
{

    VFS::Entry:: Entry(const VFS &, const String &name) :
    Object(),
    path(name),
    base( BaseName(path)  ),
    ext(  Extension(base) ),
    next(0),
    prev(0)
    {

    }


    VFS::Entry:: Entry(const VFS &, const char *name) :
    Object(),
    path( name ),
    base( BaseName(path)  ),
    ext(  Extension(base) ),
    next(0),
    prev(0)
    {

    }

    VFS::Entry:: ~Entry() noexcept
    {
    }

    static inline
    size_t shifting(const char *field, const String &path)
    {
        return field-path.c_str();
    }

    VFS::Entry:: Entry(const Entry &entry) :
    Object(),
    path(entry.path),
    base(path.c_str() + shifting(entry.base,entry.path)),
    ext(0==entry.ext? 0 : path.c_str() + shifting(entry.ext,entry.path)),
    next(0),
    prev(0)
    {
    }

}


