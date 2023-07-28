


#include "y/io/libc/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_lines)
{


    if(argc>1)
    {
        Libc::InputFile input( argv[1] );
        IO::Chars line;
        while(input.gets(line))
        {
            std::cerr << line << std::endl;
        }
    }

}
Y_UDONE()
