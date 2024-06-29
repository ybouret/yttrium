
#include "y/cipher/stream/base64/decoder.hpp"
#include "y/text/base64/decode.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Base64
    {
        const char * const Decoder:: CallSign = "Base64-Decoder";


        Decoder:: Decoder() noexcept : BufferedStreamCipher(),
        state(WaitFor1),
        input()
        {
            memset(input,0,sizeof(input));
        }

        void Decoder:: reset1() noexcept
        {
            memset(input,0,sizeof(input));
            state = WaitFor1;
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

        }

        void Decoder:: flush()
        {
            
        }

    }


}

