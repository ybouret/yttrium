#include "y/cipher/stream/base64/encoder.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;



Y_UTEST(cipher_base64enc)
{
    Base64::Encoder b64(true);

    if(argc>1)
    {
        InputFile input(argv[1]);
        b64(std::cerr,input,true);
        std::cerr << std::endl;
    }
    else
    {

        const char * const msg = "Yann";
        b64 << msg;
        b64.flush();

        std::cerr << b64.onTap() << std::endl;

        char C = 0;
        while( b64.query(C) )
        {
            std::cerr << C;
        }
        std::cerr << std::endl;


        b64.reset();
        b64(std::cerr,msg,true);
        std::cerr << std::endl;

        {
            const String s = msg;
            b64.reset();
            b64(std::cerr,s,true);
            std::cerr << std::endl;
        }
    }

}
Y_UDONE()

