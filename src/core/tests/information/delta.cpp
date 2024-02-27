#include "y/information/stream/delta.hpp"
#include "y/information/entropy.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/stream/libc/input.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;

Y_UTEST(info_delta)
{
    Information::Delta::Encoder enc;
    Information::Delta::Decoder dec;
    Information::Entropy        source;
    Information::Entropy        target;


    if(argc>1)
    {
        InputFile  inp(argv[1]);
        OutputFile out("delta.dat");
        char c = 0;
        while(inp.query(c))
        {
            const char e = enc(c);
            source << c;
            target << e;
            out.write( e );
            Y_ASSERT(dec(e)==c);
        }

        std::cerr << "input  entropy: " << source() << std::endl;
        std::cerr << "output entropy: " << target() << std::endl;

    }


}
Y_UDONE()

