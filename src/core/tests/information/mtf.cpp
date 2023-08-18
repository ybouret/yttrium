#include "y/information/stream/move-to-front.hpp"
#include "y/information/entropy.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/stream/libc/input.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;

Y_UTEST(info_mtf)
{
    Information::MoveToFront::Encoder enc;
    Information::Entropy              source;
    Information::Entropy              target;


    if(argc>1)
    {
        Libc::InputFile  inp(argv[1]);
        Libc::OutputFile out("mtf.dat");
        char c = 0;
        while(inp.query(c))
        {
            const char e = enc(c);
            source << c;
            target << e;
            out.write( e );
        }

        std::cerr << "input  entropy: " << source() << std::endl;
        std::cerr << "output entropy: " << target() << std::endl;

    }


}
Y_UDONE()

