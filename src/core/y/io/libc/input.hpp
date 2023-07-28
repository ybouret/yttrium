
//! \file

#ifndef Y_IO_Libc_Input_Included
#define Y_IO_Libc_Input_Included 1

#include "y/io/stream/input.hpp"
#include "y/io/libc/file.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }

    namespace Libc
    {
        class InputFile : public InputStream, public File
        {
        public:
            explicit InputFile(const StdIn_ &);

            virtual ~InputFile() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
        };
    }

}

#endif

