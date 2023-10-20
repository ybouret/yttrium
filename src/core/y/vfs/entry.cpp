#include "y/vfs/vfs.hpp"

namespace Yttrium
{

    VFS::Entry:: Entry(const VFS &fs, const String &name) :
    Object(),
    path(name),
    base( BaseName(path)  ),
    ext(  Extension(base) ),
    link(false),
    type( fs.findEntryType(path, Coerce(link)) ),
    next(0),
    prev(0)
    {

    }


    VFS::Entry:: Entry(const VFS &fs, const char *name) :
    Object(),
    path( name ),
    base( BaseName(path)  ),
    ext(  Extension(base) ),
    link( false ),
    type( fs.findEntryType(path, Coerce(link)) ),
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
    link(entry.link),
    type(entry.type),
    next(0),
    prev(0)
    {
    }

    bool VFS::Entry:: isReg() const noexcept  { return VFS::IsReg == type; }
    bool VFS::Entry:: isDir() const noexcept  { return VFS::IsDir == type; }


    const char * VFS::Entry:: typeText() const noexcept
    {
        return VFS::EntryTypeText(type);
    }

    std::ostream & operator<<(std::ostream &os, const VFS::Entry &ep)
    {
        os << '[';
        os << ep.typeText();
        if(ep.link)
        {
            os << '@';
        }
        else
        {
            os << ' ';
        }
        os << ']' << ' ' << ep.path;
        if(ep.ext)
        {
            os << ' ' << '[' << ep.ext << ']';
        }
        return os;
    }

}


