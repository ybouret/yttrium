//! \file

#ifndef Y_IO_Libc_CFile_Included
#define Y_IO_Libc_CFile_Included 1

#include "y/config/shallow.hpp"

namespace Yttrium
{

    namespace Core { template <typename> class String; }

    Y_SHALLOW_DECL(StdIn);
    Y_SHALLOW_DECL(StdErr);
    Y_SHALLOW_DECL(StdOut);

    namespace Libc
    {
        class File
        {
        protected:
            explicit File(void *, const bool) noexcept;

        public:
            virtual ~File() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(File);
            void      *handle;
            const bool mustClose;
        };

        
    }

}

#endif
