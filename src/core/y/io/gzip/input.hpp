
//! \file

#ifndef Y_IO_GZIP_Input_File_Imcluded
#define Y_IO_GZIP_Input_File_Imcluded 1

#include "y/io/stream/input.hpp"
#include "y/io/chars.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }

    namespace GZip
    {

        class InputFile : public InputStream
        {
        public:
            static const char * const CallSign; //!< "GZip::InputFile"

            explicit InputFile(const char *fileName);
            explicit InputFile(const Core::String<char> &fileName);
            virtual ~InputFile() noexcept;

            virtual const char * callSign() const noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
            class Code;
            Code *code;
        };

    }

}

#endif
