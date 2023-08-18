
//! \file

#ifndef Y_VFS_LocalFS_Included
#define Y_VFS_LocalFS_Included 1


#include "y/vfs/vfs.hpp"
#include "y/singleton.hpp"

namespace Yttrium
{

    class LocalFS : public Singleton<LocalFS>, public VFS
    {
    public:
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity-9;

        virtual bool TryRemove(const String &path);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LocalFS);
        friend class Singleton<LocalFS>;
        explicit LocalFS() noexcept;
        virtual ~LocalFS() noexcept;

    };
}

#endif

