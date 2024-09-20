//! \file

#ifndef Y_Stream_Disk_File_Included
#define Y_Stream_Disk_File_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! raw disk file
    //
    //
    //__________________________________________________________________________
    class DiskFile
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        class Code; //!< implementation

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~DiskFile() noexcept; //!< cleanup

    protected:
        explicit DiskFile(const char * const path); //!< create/open

    public:
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

    private:
        Y_DISABLE_COPY_AND_ASSIGN(DiskFile);
        Code *code;
    };
}

#endif

