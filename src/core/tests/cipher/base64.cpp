#include "y/cipher/stream.hpp"
#include "y/text/base64/encode.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;

namespace Yttrium
{

    namespace Base64
    {
        class Encoder : public StreamCipher
        {
        public:
            static const char * const CallSign;

            explicit Encoder() noexcept : StreamCipher() {}
            virtual ~Encoder() noexcept {}

            virtual const char * callSign() const noexcept { return CallSign; }

            virtual void write(const char C)
            {
            }
            
            virtual void flush()
            {
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Encoder);
        };
    }

}

Y_UTEST(cipher_base64)
{
    //Base64::Encoder b64;

}
Y_UDONE()

