//! \file

#ifndef Y_IO_Libc_System_Buffer_Included
#define Y_IO_Libc_System_Buffer_Included 1

#include "y/io/chars.hpp"
#include "y/io/stock.hpp"

namespace Yttrium
{

    namespace Libc
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class, adapted to BUFSIZ
        //
        //
        //______________________________________________________________________
        class SystemBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SystemBuffer();           //!< setup
            virtual ~SystemBuffer() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t   bytes; //!< bytes >= BUFSIZ
            const unsigned shift; //!< bytes=2^shift
            char * const   entry; //!< first valid char


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SystemBuffer);
        };
    }

}


#endif

