

#include "y/stream/gzip/input.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(io_gzip_read)
{
    if(argc>1)
    {
        GZip::InputFile fp(argv[1]);
        String line;
        while(fp.gets(line))
        {
            
        }
    }
}
Y_UDONE()

