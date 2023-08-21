
//! \file

#ifndef Y_VFS_Included
#define Y_VFS_Included 1


#include "y/string.hpp"
#include "y/data/list/cxx.hpp"

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
        // Entry
        //
        //______________________________________________________________________
        class Entry : public Object
        {
        public:
            explicit Entry(const VFS &, const String &);
            explicit Entry(const VFS &, const char   *);
            virtual ~Entry() noexcept;
            Entry(const Entry &);

            const String        path; //!< full   path
            const char  * const base; //!< within path
            const char  * const ext;  //!< within path
            Entry              *next;
            Entry              *prev;
            
        private:
            Y_DISABLE_ASSIGN(Entry);
        };

        typedef CxxListOf<Entry> Entries;


        class Scanner : public Object
        {
        public:
            virtual ~Scanner() noexcept;

            virtual Entry *get() = 0;

        protected:
            explicit Scanner(const VFS &) noexcept;
            const VFS &fs;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scanner);
        };

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool     TryRemove(const String &path) = 0; //!< try to remove file from VFS
        bool             TryRemove(const char   *path);     //!< alias
        virtual Scanner *OpenDirectory(const String &dirName) = 0;
        virtual Scanner *OpenDirectory(const char   *dirName);

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
