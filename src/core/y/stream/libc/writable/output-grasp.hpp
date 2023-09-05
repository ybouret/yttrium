//! \file

#ifndef Y_IO_Libc_OutputGrasp_Included
#define Y_IO_Libc_OutputGrasp_Included 1

#include "y/stream/libc/file.hpp"

namespace Yttrium
{


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
            void *openOut();
            void *openErr();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(OutputGrasp);
        };

    }

}

#endif

