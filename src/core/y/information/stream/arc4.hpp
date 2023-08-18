//! \file

#ifndef Y_Information_ARC4_Included
#define Y_Information_ARC4_Included 1

#include "y/information/stream/cipher.hpp"

namespace Yttrium
{
    namespace Information
    {
        struct ARC4
        {
            class Code;

            class Encoder : public StreamCipher
            {
            public:
                virtual ~Encoder() noexcept;
                explicit Encoder(const void *key, const size_t keyLength);
                explicit Encoder(const char *key);

                virtual void    restart()                 noexcept;
                virtual uint8_t operator()(const uint8_t) noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                Code *code;
            };

        };
    }

}

#endif
