
//! \file

#ifndef Y_VFS_Included
#define Y_VFS_Included 1


#include "y/string.hpp"

namespace Yttrium
{
    class VFS
    {
    public:
        static const char  EmptyName[];
        static bool        IsSeparator(const char c) noexcept;
        static const char *BaseName(const char * const path, const size_t size)  noexcept;
        static const char *BaseName(const char * const path)                     noexcept;
        static const char *BaseName(const String     & path)                     noexcept;

        static const char *Extension(const char * const path, const size_t size) noexcept;
        static const char *Extension(const char * const path)                    noexcept;
        static const char *Extension(const String &     path)                    noexcept;


        virtual ~VFS() noexcept;
    protected:
        explicit  VFS() noexcept;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(VFS);
    };
}

#endif
