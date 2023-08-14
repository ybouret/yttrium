//! \file

#ifndef Y_IO_GZIP_FILE_INCLUDED
#define Y_IO_GZIP_FILE_INCLUDED 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace GZip
    {

        //______________________________________________________________________
        //
        //
        //
        //! wrapper around gzFile
        //
        //
        //______________________________________________________________________
        class File
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit File(const char *filename, const char *options); //!< gzopen
            virtual ~File() noexcept;                                 //!< gzclose

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________
            unsigned read(void *buff, const unsigned size); //!< reading bytes with gzread
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(File);
            void *handle;

        };
    }
}

#endif

