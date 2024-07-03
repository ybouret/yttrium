
#include "y/codec/base64/decoder.hpp"
#include "y/text/base64/decode.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Base64
    {
        const char * const Decoder:: CallSign = "Base64-Decoder";


        Decoder:: Decoder() noexcept : BufferedCodec(),
        state(0),
        input()
        {
            memset(input,0,sizeof(input));
        }

        void Decoder:: reset1() noexcept
        {
            memset(input,0,sizeof(input));
            state = 0;
        }

        Decoder:: ~Decoder() noexcept
        {
        }

        const char * Decoder:: callSign() const noexcept
        {
            return CallSign;
        }


        void Decoder:: reset() noexcept
        {
            clearBuffer();
            reset1();
        }

        void Decoder:: write(const char c)
        {
            assert(state<4);

            switch(c)
            {
                case ' ':
                case '\n':
                case '\t':
                case '\r':
                    return;
                    
                default:
                    break;
            }

            input[state++] = c;
            if(state>=4)
                flush();
        }


        static inline
        void Store(IO::Chars &io, const uint8_t *code, size_t size)
        {
            while(size-- > 0) io << *(code++);
        }

        void Decoder:: flush()
        {
            uint8_t code[4] = {0,0,0,0};
        DECODE:
            switch(state)
            {
                case 0: return;

                case 2:
                    Decode::_1(code, input[0], input[1]);
                    break;

                case 3:
                    Decode::_2(code, input[0], input[1], input[2]);
                    break;

                case 4:
                    if(Padding == input[3])
                    {
                        if( Padding == input[2] )
                        {
                            state = 2;
                            goto DECODE;
                        }
                        else
                        {
                            state = 3;
                            goto DECODE;
                        }
                    }
                    else
                    {
                        Decode::_3(code, input[0], input[1], input[2], input[3]);
                    }
                    break;

                default:
                    throw Specific::Exception(CallSign, "invalid input size=%u",state);

            }
            assert(state>1);
            Store(buffer,code,--state);
            reset1();
        }

    }


}

