

//! \file

#ifndef Y_IO_Libc_Output_Included
#define Y_IO_Libc_Output_Included 1

#include "y/io/stream/output.hpp"
#include "y/io/libc/file.hpp"
#include "y/io/libc/buffer/direct.hpp"

namespace Yttrium
{
    namespace Core { template <typename> class String; }


    //__________________________________________________________________________
    //
    //! Hard-coded file name redirecting to stderr
    //__________________________________________________________________________
#define Y_STDERR ":STDERR:"

    //__________________________________________________________________________
    //
    //! Hard-coded file name redirecting to stderr
    //__________________________________________________________________________
#define Y_STDOUT ":STDOUT:"

    namespace Libc
    {
        class OutputGrasp
        {

        protected:
            explicit OutputGrasp(const StdErr_ &) noexcept;
            explicit OutputGrasp(const StdOut_ &) noexcept;
            explicit OutputGrasp(const char *   );

        public:
            virtual ~OutputGrasp() noexcept;

            const bool isErr;
            const bool isOut;
            const bool isReg;

        protected:
            void *openFile(const char *, const bool append);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputGrasp);
        };


        class OutputFile : public OutputStream, public OutputGrasp, public File
        {
        public:
            virtual ~OutputFile() noexcept;

            explicit OutputFile(const StdErr_ &);
            explicit OutputFile(const StdOut_ &);
            explicit OutputFile(const char *fileName, const bool append=false);
            explicit OutputFile(const Core::String<char> &fileName);


            virtual void write(const char);
            virtual void flush();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
            Libc::DirectBuffer buffer;
            void emit();
        };


    }

}


#endif
