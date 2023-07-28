
#include "y/io/libc/output.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(io_ostream)
{
    Libc::OutputFile fp( StdOut );
}
Y_UDONE()
