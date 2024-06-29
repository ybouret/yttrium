
#include "y/cipher/stream/base64/decoder.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;


Y_UTEST(cipher_base64dec)
{
    Base64::Decoder u64;
    
    if(argc>1)
    {
        InputFile fp(argv[1]);

        u64(std::cout,fp,true);

    }

}
Y_UDONE()
