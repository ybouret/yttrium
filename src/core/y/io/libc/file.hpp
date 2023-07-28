//! \file

#ifndef Y_IO_Libc_CFile_Included
#define Y_IO_Libc_CFile_Included 1

#include "y/config/shallow.hpp"

namespace Yttrium
{


    Y_SHALLOW_DECL(StdIn);   //!< alias
    Y_SHALLOW_DECL(StdErr);  //!< alias
    Y_SHALLOW_DECL(StdOut);  //!< alias

    namespace Libc
    {
        //! FILE wrapper
        class File
        {
        protected:
            //! setup from FILE * and closing flag
            explicit File(void *, const bool) noexcept;

        public:
            //! auto-close
            virtual ~File() noexcept;

        protected:
            void      *handle;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(File);
            const bool mustClose;
        };

        
    }

}

#endif
