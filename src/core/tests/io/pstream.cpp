
#include "y/utest/run.hpp"
#include "y/stream/proc/input.hpp"

#if defined(Y_WIN)
#if   defined(_MSC_VER)
#       define WIN32_LEAN_AND_MEAN
#       include <windows.h>
#       define popen  _popen
#       define pclose _pclose
#else // Y_WIN
extern "C" FILE * popen(const char *, const char *);
extern "C" void   pclose(FILE *);
#endif // Y_WIN
#else
#include <cstdio>
#endif

using namespace Yttrium;


Y_UTEST(io_pstream)
{
    if(argc>1)
    {
        const String cmd = argv[1];

        ProcInput    inp( cmd );
        String       line;
        while( inp.gets(line) )
        {
            std::cerr << "(+) '" << line << "'" << std::endl;
        }
#if 0
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
#endif
    }

}
Y_UDONE()

