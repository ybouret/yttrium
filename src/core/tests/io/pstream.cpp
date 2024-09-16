
#include "y/utest/run.hpp"
#include "y/stream/input.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(io_pstream)
{
    if(argc>1)
    {
        const String cmd = argv[1];
        std::cerr << "trying '" << cmd << "'" << std::endl;
        FILE *fp = popen( cmd.c_str(), "r" );
        if(0==fp) throw Exception("no '%s'", cmd.c_str() );

        char   buffer[256];
        size_t buflen = 0;

        while( 0 != (buflen=fread(buffer, 1, sizeof(buffer), fp)) )
        {
            fwrite(buffer,1,buflen,stdout);
            fflush(stdout);
        }


        pclose(fp);
    }

}
Y_UDONE()

