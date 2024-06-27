
#include "y/cipher/stream/base64/encoder.hpp"
#include "y/text/base64/encode.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Base64
    {
        const char * const Encoder:: CallSign = "Base64-Encoder";


        Encoder:: Encoder(bool padding) noexcept :
        BufferedStreamCipher(),
        doPad(padding),
        state(WaitFor1),
        input(),
        table( Base64::Encode::Table )
        {}

        Encoder:: ~Encoder() noexcept
        {
            waitFor1();
        }

        const char * Encoder:: callSign() const noexcept { return CallSign; }


        void Encoder:: waitFor1() noexcept
        {
            state = WaitFor1;
            memset(input,0,sizeof(input));
        }

        void  Encoder:: flush1() {
            char output[4] = {0,0,0,0};
            send(output,Encode::_1(output, input[0], table, doPad));
        }

        void Encoder:: flush2() {
            char output[4] = {0,0,0,0};
            send(output,Encode::_2(output, input[0], input[1], table, doPad));
        }

        void Encoder:: flush3() {
            char output[4] = {0,0,0,0};
            send(output,Encode::_3(output, input[0], input[1], input[2],table));
        }

        void Encoder:: send(const char *p, size_t n)
        {
            IO::Chars out;
            while(n-- > 0 ) out << *(p++);
            buffer.mergeTail(out);
            waitFor1();
        }


        void Encoder:: write(const char C)
        {
            switch(state)
            {
                case WaitFor1: input[0] = C; state = WaitFor2; break;
                case WaitFor2: input[1] = C; state = WaitFor3; break;
                case WaitFor3: input[2] = C; flush3(); break;
            }
        }


        void Encoder:: flush()
        {
            switch(state)
            {
                case WaitFor1: break;
                case WaitFor2: flush1(); break;
                case WaitFor3: flush2(); break;
            }
            assert(WaitFor1==state);
        }

        void Encoder:: reset() noexcept
        {
            buffer.release();
            waitFor1();
        }
    }

}

