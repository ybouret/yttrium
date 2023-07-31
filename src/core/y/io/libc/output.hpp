

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
        //______________________________________________________________________
        //
        //
        //
        //! Grasp kind of output
        //
        //
        //______________________________________________________________________
        class OutputGrasp
        {

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit OutputGrasp(const StdErr_ &) noexcept; //!< StdErr
            explicit OutputGrasp(const StdOut_ &) noexcept; //!< StdOut
            explicit OutputGrasp(const char *   );          //!< check name

        public:
            virtual ~OutputGrasp() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const bool isErr; //!< -> stderr
            const bool isOut; //!< -> stdout
            const bool isReg; //!< -> fopen

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
        protected:
            //! act according to flags
            void *openFile(const char *, const bool append);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputGrasp);
        };

        //______________________________________________________________________
        //
        //
        //
        //! OutputStream based on a C FILE
        //
        //
        //______________________________________________________________________
        class OutputFile : public OutputStream, public OutputGrasp, public File
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Libc::OutputFile"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~OutputFile() noexcept;       //!< cleanup
            explicit OutputFile(const StdErr_ &); //!< stderr
            explicit OutputFile(const StdOut_ &); //!< stdout

            //! open regular files of Y_STDERR/Y_STDOUT
            explicit OutputFile(const char *fileName, const bool append=false);

            //! open regular files of Y_STDERR/Y_STDOUT
            explicit OutputFile(const Core::String<char> &fileName, const bool append=false);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void write(const char); //!< emit/write to buffer
            virtual void flush();           //!< emit
            virtual const char * callSign() const throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputFile);
            Libc::DirectBuffer buffer;
            void emit();
        };


    }

}


#endif
