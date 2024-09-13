//! \file

#ifndef Y_Stream_Disk_File_Included
#define Y_Stream_Disk_File_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    class DiskFile
    {
    public:
        class Code;
        virtual ~DiskFile() noexcept;

    protected:
        explicit DiskFile(const char * const path);
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(DiskFile);
        Code *code;
    };
}

#endif

