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

        class ReadableFile : public IsStdIn, public File
        {
        public:
            virtual ~ReadableFile() noexcept;
            
        protected:
            explicit ReadableFile(const StdIn_ &);
            explicit ReadableFile(const char *  );
            explicit ReadableFile(const Core::String<char> &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadableFile);
        };

    }
}


#endif

