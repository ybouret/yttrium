//! \file

#ifndef Y_Stream_Disk_File_Included
#define Y_Stream_Disk_File_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    class DiskFile
    {
    public:
        virtual ~DiskFile() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(DiskFile);
    };
}

#endif

