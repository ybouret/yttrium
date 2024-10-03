
//! \file

#ifndef Y_VFS_LocalFS_Included
#define Y_VFS_LocalFS_Included 1


#include "y/vfs/vfs.hpp"
#include "y/singleton.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Local File System Interface
    //
    //
    //__________________________________________________________________________
    class LocalFS : public Singleton<LocalFS>, public VFS
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const      CallSign;                              //!< "LocalFS"
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity-9; //!< alias

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual bool      tryRemoveFile(const String &path);                         //!< try to remove file from disk
        virtual Scanner * openDirectory(const String &dirName);                      //!< use O/S functions
        virtual EntryType findEntryType(const String &path, bool &link) const;       //!< use O/S functions
        virtual void      makeDirectory(const String &dirName, const bool mayExist); //!< use O/S functions
        virtual bool      tryEraseEmpty(const String &dirName);                      //!< use O/S functions
        virtual String    getCWD();                                                  //!< use O/S functions
        virtual void      setCWD(const String &dirName);                             //!< use O/S functions
        static  String    MakeWin32Path(const String &dirName);                      //!< needed for Win32

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LocalFS);
        friend class Singleton<LocalFS>;
        explicit LocalFS() noexcept;
        virtual ~LocalFS() noexcept;

    };
}

#endif

