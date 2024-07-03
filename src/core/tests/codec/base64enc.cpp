#include "y/codec/base64/encoder.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;



Y_UTEST(codec_b64)
{
    Base64::Encoder b64(true);

    if(argc>1)
    {
        InputFile input(argv[1]);
        b64(std::cout,input,true);
        std::cout << std::endl;
    }
    else
    {

        const char * const msg = "Yann";
        b64 << msg;
        b64.flush();

        std::cerr << b64.buffered() << std::endl;

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

