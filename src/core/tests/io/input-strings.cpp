#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/stream/data/input.hpp"

using namespace Yttrium;

Y_UTEST(io_strings)
{

    if(argc>1)
    {
        InputFile input( argv[1] );
        String    line;
        while(input.gets(line))
        {
            std::cerr << line;

            InputDataStream sc(line);
            std::cerr << " '";
            char C=0;
            while( sc.query(C) ) std::cerr << C;
            std::cerr << "'" << std::endl;

        }
    }

}
Y_UDONE()
