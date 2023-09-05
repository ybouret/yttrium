#include "y/stream/bzip2/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_bzip2_read)
{
    if(argc>1)
    {
        Bzip2::InputFile inp(argv[1]);
        IO::Chars line;
        while(inp.gets(line))
        {
            (std::cerr << line << std::endl).flush();
        }
    }
}
Y_UDONE()
