
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
        static String ChangedExtension(const char * const newExt,
                                       const size_t       extLen,
                                       const char * const path,
                                       const size_t       size);
        static String ChangedExtension(const char * const  newExt, const String & path); //!< change extension
        static String ChangedExtension(const String      & newExt, const String & path); //!< change extension


        static String RemovedExtension(const char * const path, const size_t size); //!< path with removed extension
        static String RemovedExtension(const String& path);                         //!< path with removed extension


        //______________________________________________________________________
        //
        //
        // Directory Name
        //
        //______________________________________________________________________

        //! make simplified, separator ending directory name
        static String MakeDirName(const char *const path, const size_t size);
        static String MakeDirName(const char *const path); //!< alias
        static String MakeDirName(const String &    path); //!< alias


        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char  EmptyName[]; //!< ""

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
            //______________________________________________________________________
            //
            //! for file name operations
            //______________________________________________________________________
            enum Part
            {
                Path,      //!< use path
                Base,      //!< use base name
                PathWE,    //!< use path without extension
                BaseWE,    //!< use base without extension
                Ext        //!< use extension
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Entry(const VFS &, const String &); //!< setup
            explicit Entry(const VFS &, const char   *); //!< setup
            virtual ~Entry() noexcept;                   //!< cleanup
            Entry(const Entry &);                        //!< duplicate
            Y_OSTREAM_PROTO(Entry);                      //!< full display

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const char * typeText() const noexcept; //!< EntryTypeText(type)
            bool         isReg()    const noexcept; //!< type == IsReg
            bool         isDir()    const noexcept; //!< type == IsDir
            String       pry(const Part)     const; //!< convert to string for ops

            //! lexicographic path comparison
            static SignType CompareByPath(const Entry *lhs, const Entry *rhs) noexcept;

            //! lexicographic base comparison
            static SignType CompareByName(const Entry *lhs, const Entry *rhs) noexcept;


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
            explicit       Scanner(const VFS &,const String &) ; //!< setup
            const VFS     &vfs;                                  //!< original vfs
            const String   dir;                                  //!< original directory

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scanner);
        };

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual bool      tryRemoveFile(const String &path)                         = 0; //!< try to remove file from VFS
        virtual Scanner * openDirectory(const String &dirName)                      = 0; //!< create scanner for dirName
        virtual EntryType findEntryType(const String &path, bool &lnk) const        = 0; //!< get entry attributes
        virtual void      makeDirectory(const String &dirName, const bool mayExist) = 0; //!< create a directory
        virtual bool      tryEraseEmpty(const String &dirName)                      = 0; //!< try to erase an empty directory

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        bool         tryRemoveFile(const char * const path);                         //!< alias
        Scanner    * openDirectory(const char * const dirName);                      //!< alias
        EntryType    findEntryType(const char * const path, bool &lnk) const;        //!< alias
        void         makeDirectory(const char * const dirName, const bool mayExist); //!< alias
        bool         tryEraseEmpty(const char * const dirName);                      //!< alias

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
