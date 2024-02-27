
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(io_copy)
{
    if(argc>1)
    {
        InputFile  input(argv[1]);
        OutputFile output("copy.dat");
        char C = 0;
        while( input.query(C) )
        {
            output.write(C);
        }
        output.flush();
    }
}
Y_UDONE()
