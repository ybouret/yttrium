


#include "y/stream/libc/input.hpp"
#include "y/stream/char/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_lines)
{

    if(argc>1)
    {
        InputFile input( argv[1] );
        IO::Chars line;
        while(input.gets(line))
        {
            std::cerr << line;
            InputCharStream sc(line);
            std::cerr << " '";
            char C=0;
            while( sc.query(C) ) std::cerr << C;
            std::cerr << "'" << std::endl;
        }
    }

}
Y_UDONE()
