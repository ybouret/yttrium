
#include "y/codec/base64/decoder.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;


Y_UTEST(codec_u64)
{
    Base64::Decoder u64;
    
    if(argc>1)
    {
        InputFile fp(argv[1]);

        u64(std::cout,fp,true);

    }

}
Y_UDONE()
