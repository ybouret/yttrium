//! \file

#ifndef Y_IO_Libc_Direct_Buffer_Included
#define Y_IO_Libc_Direct_Buffer_Included 1

#include "y/stream/libc/buffer/system.hpp"

namespace Yttrium
{

    namespace Libc
    {

        //______________________________________________________________________
        //
        //
        //
        //! buffer with direct access for output file
        //
        //
        //______________________________________________________________________
        class DirectBuffer : public SystemBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit DirectBuffer();           //!< setup
            virtual ~DirectBuffer() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            char       *       curr; //!< in buffer.entry..last
            const char * const last; //!< buffer.entry + buffer.bytes


        private:
            Y_DISABLE_COPY_AND_ASSIGN(DirectBuffer);
        };
    }

}


#endif
