
#include "y/cipher/stream/base64/decoder.hpp"
#include "y/text/base64/decode.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Base64
    {
        const char * const Decoder:: CallSign = "Base64-Decoder";


        Decoder:: Decoder() noexcept : BufferedStreamCipher(),
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
            buffer.release();

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

            std::cerr << "@state=" << state << " -> '" << c << "'" << std::endl;

            input[state++] = c;

            // TODO: check padding

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
                    Decode::_3(code, input[0], input[1], input[2], input[3]);
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

