
#include "y/io/libc/output.hpp"
#include "y/io/libc/input.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(io_copy)
{
    if(argc>1)
    {
        Libc::InputFile  input(argv[1]);
        Libc::OutputFile output("copy.dat");
        char C = 0;
        while( input.query(C) )
        {
            output.write(C);
        }
        output.flush();
    }
}
Y_UDONE()
