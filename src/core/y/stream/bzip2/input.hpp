//! \file

#ifndef Y_Stream_Bzip2_InputFile_Included
#define Y_Stream_Bzip2_InputFile_Included 1

#include "y/stream/input.hpp"

namespace Yttrium
{
    
    namespace Bzip2
    {

        class InputFile : public InputStream
        {
        public:
            explicit InputFile(const char *               fileName);
            explicit InputFile(const Core::String<char> & fileName);
            virtual ~InputFile() noexcept;

        private:
            class Code;
            Code *code;

        };
    }
}

#endif

