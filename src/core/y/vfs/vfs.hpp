
//! \file

#ifndef Y_VFS_Included
#define Y_VFS_Included 1


#include "y/string.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! VFS Interface
    //
    //
    //__________________________________________________________________________
    class VFS
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char  EmptyName[]; //!< ""

        //______________________________________________________________________
        //
        //
        // File Names
        //
        //______________________________________________________________________
        static bool        IsSeparator(const char c) noexcept; //!< [back]slash
        static const char *BaseName(const char * const path, const size_t size)  noexcept; //!< basename
        static const char *BaseName(const char * const path)                     noexcept; //!< basename
        static const char *BaseName(const String     & path)                     noexcept; //!< basename

        static const char *Extension(const char * const path, const size_t size) noexcept; //!< NULL or .ext
        static const char *Extension(const char * const path)                    noexcept; //!< NULL or .ext
        static const char *Extension(const String &     path)                    noexcept; //!< NULL or .ext


        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool TryRemove(const String &path) = 0;
        bool         TryRemove(const char   *path);
        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        virtual ~VFS() noexcept; //!< cleanup
    protected:
        explicit VFS() noexcept; //!< setup
    private:
        Y_DISABLE_COPY_AND_ASSIGN(VFS);
    };
}

#endif
