#include "y/cipher/stream/base64/encoder.hpp"

#include "y/utest/run.hpp"
#include <cstring>


using namespace Yttrium;



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

