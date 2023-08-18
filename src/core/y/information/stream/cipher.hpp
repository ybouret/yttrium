//! \file

#ifndef Y_Information_Stream_Cipher_Included
#define Y_Information_Stream_Cipher_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Information
    {
        //______________________________________________________________________
        //
        //
        //
        //! Stream Cipher interface
        //
        //
        //______________________________________________________________________
        class StreamCipher
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual uint8_t operator()(const uint8_t) noexcept = 0; //!< get next byte
            virtual void    restart()                 noexcept = 0; //!< restart cipher

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~StreamCipher() noexcept; //!< cleanup
        protected:
            explicit StreamCipher() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);
        };

    }
}

#endif

