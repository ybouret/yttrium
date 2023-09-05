#ifndef Y_Stream_Readable_IsStdin_Included
#define Y_Stream_Readable_IsStdin_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //! Hard-coded file name redirecting to stdin
    //__________________________________________________________________________
#define Y_STDIN ":STDIN:"

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //! analyze file name of assuming stdin
        //
        //______________________________________________________________________
        class IsStdIn
        {
        public:
            virtual ~IsStdIn() noexcept;    //!< cleanup

        protected:
            explicit IsStdIn() noexcept;    //!< flag = true
            explicit IsStdIn(const char *); //!< check valid file name and stdin
            const bool flag;                //!< true if stdin

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IsStdIn);
        };

    }

}

#endif
