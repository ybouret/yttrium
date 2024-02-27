
#include "y/stream/bzip2/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_bzip2_write)
{
    if(argc>1)
    {
        Bzip2::OutputFile out("output.bz2",9);
        InputFile         inp(argv[1]);
        char C = 0;
        while(inp.query(C))
            out.write(C);

    }
}
Y_UDONE()
