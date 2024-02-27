

#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_istream)
{

    { InputFile trial(StdIn); }

    if(argc>1)
    {
        InputFile input( argv[1] );
        char C;
        while( input.query(C) )
        {
            std::cerr << '<' << C << '>';
        }
    }

}
Y_UDONE()
