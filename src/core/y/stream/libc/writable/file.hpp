
//! \file

#ifndef Y_IO_Libc_WritableFile_Included
#define Y_IO_Libc_WritableFile_Included 1

#include "y/stream/libc/file.hpp"
#include "y/stream/libc/writable/output-grasp.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //
        //! make a writable FILE
        //
        //
        //______________________________________________________________________
        class WritableFile : public OutputGrasp, public File
        {
        public:
            virtual ~WritableFile() noexcept; //!< cleanup

        protected:
            explicit WritableFile(const StdOut_ &);                                       //!< link to stdout
            explicit WritableFile(const StdErr_ &);                                       //!< link to stderr
            explicit WritableFile(const char *fileName, const bool append);               //!< open file, append
            explicit WritableFile(const Core::String<char> &fileName, const bool append); //!< open file, append
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(WritableFile);
        };
    }
}

#endif

