
//! \file

#ifndef Y_IO_GZIP_FILE_INCLUDED
#define Y_IO_GZIP_FILE_INCLUDED 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace GZip
    {
        class File
        {
        public:
            virtual ~File() noexcept;

        protected:
            explicit File(const char *filename, const char *options);
            void *handle;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(File);

        };
    }
}

#endif

