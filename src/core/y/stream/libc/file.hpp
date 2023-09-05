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
        //______________________________________________________________________
        //
        //
        //! FILE wrapper
        //
        //______________________________________________________________________
        class File
        {
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
        protected:
            //! setup from FILE * and closing flag
            explicit File(void *, const bool) noexcept;

        public:
            //! auto-close
            virtual ~File() noexcept;

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            void * const handle; //!< anonymous handle

        private:
            Y_DISABLE_COPY_AND_ASSIGN(File);
            const bool mustClose;
        };

        
    }

}

#endif
