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

            explicit Encoder() noexcept : 
            BufferedStreamCipher(),
            count(0),
            input()
            {}

            virtual ~Encoder() noexcept {}

            virtual const char * callSign() const noexcept { return CallSign; }


            virtual void write(const char C)
            {
                
            }

            virtual void flush() noexcept
            {
            }



            virtual void reset() noexcept
            {
                buffer.release();
                count = 0;
                memset(input,0,sizeof(input));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            size_t  count;
            char    input[4];

        };

        const char * const Encoder:: CallSign = "Base64-Encoder";
    }

}

Y_UTEST(cipher_base64)
{
    Base64::Encoder b64;

}
Y_UDONE()

