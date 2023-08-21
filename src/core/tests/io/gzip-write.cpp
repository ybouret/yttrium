
#include "y/stream/gzip/output.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;



Y_UTEST(io_gzip_write)
{

    static const char txt[] = "The quick brown fox jumps over the lazy dog";

    {
        GZip::OutputFile fp("gzout.dat.gz",6);
        fp << txt;
        fp << '\n';
    }

    {
        GZip::OutputFile fp("gzout.dat.gz",9,true);
        fp << txt;
        fp << '\n';
    }

}
Y_UDONE()

