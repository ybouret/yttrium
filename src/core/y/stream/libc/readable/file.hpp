//! \file

#ifndef Y_Stream_Readable_File_Included
#define Y_Stream_Readable_File_Included 1

#include "y/stream/libc/readable/is-stdin.hpp"
#include "y/stream/libc/file.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    namespace Libc
    {

        //______________________________________________________________________
        //
        //
        //
        //! provide a readable FILE
        //
        //
        //______________________________________________________________________
        class ReadableFile : public IsStdIn, public File
        {
        public:
            virtual ~ReadableFile() noexcept; //!< cleanup
            
        protected:
            explicit ReadableFile(const StdIn_ &);             //!< force     STDIN
            explicit ReadableFile(const char *  );             //!< file of Y_STDIN
            explicit ReadableFile(const Core::String<char> &); //!< file or Y_STDIN

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadableFile);
        };

    }
}


#endif

