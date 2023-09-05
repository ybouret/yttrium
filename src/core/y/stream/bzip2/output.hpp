

//! \file

#ifndef Y_Stream_Bzip2_OutputFile_Included
#define Y_Stream_Bzip2_OutputFile_Included 1

#include "y/stream/output.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    namespace Bzip2
    {

        class OutputFile : public OutputStream
        {
        public:
            static const char * const CallSign;

            virtual ~OutputFile() noexcept;
            explicit OutputFile(const char *               fileName, const int level);
            explicit OutputFile(const Core::String<char> & filename, const int level);

            virtual const char * callSign() const noexcept;
            virtual void         write(const char);
            virtual void         flush();
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
            class Code;
            Code *code;
        };
    }
}

#endif

