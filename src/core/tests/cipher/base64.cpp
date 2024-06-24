#include "y/cipher/stream/buffered.hpp"
#include "y/text/base64/encode.hpp"

#include "y/utest/run.hpp"
#include <cstring>


using namespace Yttrium;

namespace Yttrium
{

    namespace Base64
    {
        class Encoder : public BufferedStreamCipher
        {
        public:
            static const char * const CallSign;

            enum State
            {
                WaitFor1,
                WaitFor2,
                WaitFor3
            };

            explicit Encoder(bool padding) noexcept :
            BufferedStreamCipher(),
            doPad(padding),
            state(WaitFor1),
            input()
            {}

            virtual ~Encoder() noexcept {}

            virtual const char * callSign() const noexcept { return CallSign; }


            virtual void write(const char C)
            {
                switch(state)
                {
                    case WaitFor1: input[0] = C; state = WaitFor2; break;
                    case WaitFor2: input[1] = C; state = WaitFor3; break;
                    case WaitFor3: input[2] = C; flush3(); break;
                }
            }

            virtual void flush() noexcept
            {
                switch(state)
                {
                    case WaitFor1: break;
                    case WaitFor2: flush1(); break;
                    case WaitFor3: flush2(); break;
                }
                assert(WaitFor1==state);
            }


            virtual void reset() noexcept
            {
                buffer.release();
                waitFor1();
            }

            bool   doPad;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            State  state;
            char   input[4];


            void   waitFor1() noexcept
            {
                state = WaitFor1;
                memset(input,0,sizeof(input));
            }

            void flush1() {
                char output[4] = {0,0,0,0};
                send(output,Encode::_1(output, input[0], doPad));
            }

            void flush2() {
                char output[4] = {0,0,0,0};
                send(output,Encode::_2(output, input[0], input[1], doPad));
            }

            void   flush3() {
                char output[4] = {0,0,0,0};
                send(output,Encode::_3(output, input[0], input[1], input[2]));
            }

            void send(const char *p, size_t n)
            {
                IO::Chars out;
                while(n-- > 0 ) out << *(p++);
                buffer.mergeTail(out);
                waitFor1();
            }

        };

        const char * const Encoder:: CallSign = "Base64-Encoder";
    }

}

Y_UTEST(cipher_base64)
{
    Base64::Encoder b64(true);

    b64 << "Yann";
    b64.flush();

    std::cerr << b64.onTap() << std::endl;

    char C = 0;
    while( b64.query(C) )
    {
        std::cerr << C;
    }
    std::cerr << std::endl;


}
Y_UDONE()

