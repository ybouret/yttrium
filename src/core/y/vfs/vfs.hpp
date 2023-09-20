
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

        //! changing extension
        static String ChangeExtension(const char * const newExt,
                                      const size_t       extLen,
                                      const char * const path,
                                      const size_t       size);
        static String ChangeExtension(const char * const  newExt, const String & path); //!< change extension
        static String ChangeExtension(const String      & newExt, const String & path); //!< change extension

        //! admitted entry type
        enum EntryType
        {
            IsUnk, //!< doesn't exists ?
            IsReg, //!< regular file
            IsDir  //!< directory
        };

        //! human readable entry type
        static const char *EntryTypeText(const EntryType) noexcept;

        //______________________________________________________________________
        //
        //
        //! Entry
        //
        //______________________________________________________________________
        class Entry : public Object
        {
        public:


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Entry(const VFS &, const String &); //!< setup
            explicit Entry(const VFS &, const char   *); //!< setup
            virtual ~Entry() noexcept;                   //!< cleanup
            Entry(const Entry &);                        //!< duplicate

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const char * typeText() const noexcept; //!< EntryTypeText(type)
            Y_OSTREAM_PROTO(Entry);                 //!< full display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const String        path; //!< full   path
            const char  * const base; //!< within path
            const char  * const ext;  //!< within path
            const bool          link; //!< flag for symbolic link
            const EntryType     type; //!< from fs
            Entry              *next; //!< for list
            Entry              *prev; //!< fir list
            
        private:
            Y_DISABLE_ASSIGN(Entry);
        };

        //______________________________________________________________________
        //
        //
        //! List of Entries
        //
        //______________________________________________________________________
        typedef CxxListOf<Entry> Entries;


        //______________________________________________________________________
        //
        //
        //! Directory Scanner
        //
        //______________________________________________________________________
        class Scanner : public Object
        {
        public:
            virtual       ~Scanner() noexcept;            //!< cleanup
            virtual Entry *get()          = 0;            //!< get next entry
        protected:
            explicit       Scanner(const VFS &) noexcept; //!< setup
            const VFS     &fs;                            //!< original vfs
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scanner);
        };

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool      tryRemove(const String &path) = 0;           //!< try to remove file from VFS
        bool              tryRemove(const char   *path);               //!< alias
        virtual Scanner * openDirectory(const String &dirName)    = 0; //!< create scanner for dirName
        Scanner         * openDirectory(const char   *dirName);        //!< alias
        virtual EntryType findEntryType(const String &path, bool &lnk) const = 0; //!< get entry attributes
        EntryType         findEntryType(const char   *path, bool &lnk) const;     //!< alias


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
