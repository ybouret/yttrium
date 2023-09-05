
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

        class WritableFile : public OutputGrasp, public File
        {
        public:
            virtual ~WritableFile() noexcept;

        protected:
            explicit WritableFile(const StdOut_ &);
            explicit WritableFile(const StdErr_ &);
            explicit WritableFile(const char *fileName, const bool append);
            explicit WritableFile(const Core::String<char> &fileName, const bool append);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(WritableFile);
        };
    }
}

#endif

