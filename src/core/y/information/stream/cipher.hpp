//! \file

#ifndef Y_Information_Stream_Cipher_Included
#define Y_Information_Stream_Cipher_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Information
    {

        class StreamCipher
        {
        public:
            virtual ~StreamCipher() noexcept;

            virtual uint8_t operator()(const uint8_t) noexcept = 0;
            virtual void    restart()                 noexcept = 0;


        protected:
            explicit StreamCipher() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(StreamCipher);
        };

    }
}

#endif

