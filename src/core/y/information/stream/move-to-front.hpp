
//! \file

#ifndef Y_Information_Move_To_Front_Included
#define Y_Information_Move_To_Front_Included 1

#include "y/information/stream/cipher.hpp"

namespace Yttrium
{
    namespace Information
    {

        struct MoveToFront
        {
            class Code;

            class Common : public StreamCipher
            {
            public:
                virtual ~Common() noexcept;

                virtual void restart() noexcept;

            protected:
                explicit Common();
                Code *code;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Common);
            };

            class Encoder : public Common
            {
            public:
                explicit Encoder();
                virtual ~Encoder() noexcept;

                virtual uint8_t operator()(const uint8_t ) noexcept;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

            class Decoder : public Common
            {
            public:
                explicit Decoder();
                virtual ~Decoder() noexcept;

                virtual uint8_t operator()(const uint8_t ) noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
            };
        };

    }

}

#endif
