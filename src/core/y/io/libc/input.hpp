
//! \file

#ifndef Y_IO_Libc_Input_Included
#define Y_IO_Libc_Input_Included 1

#include "y/io/stream/input.hpp"
#include "y/io/libc/file.hpp"
#include "y/io/libc/iobuffer.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }

#define Y_STDIN ":STDIN:"

    namespace Libc
    {
        class IsStdIn
        {
        public:
            virtual ~IsStdIn() noexcept;

        protected:
            explicit IsStdIn() noexcept;    //!< flag = true
            explicit IsStdIn(const char *); //!< check valid file name
            const bool flag;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IsStdIn);
        };

        class InputFile : public InputStream, public IsStdIn, public File
        {
        public:
            virtual ~InputFile() noexcept;
            explicit InputFile(const StdIn_ &);
            explicit InputFile(const char *fileName);


            virtual bool query(char &);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputFile);
            IOBuffer buffer;
        };
    }

}

#endif

