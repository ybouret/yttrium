

#include "y/io/libc/iobuffer.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/singulet.hpp"

using namespace Yttrium;

Y_UTEST(io_buffer)
{
    Concurrent::Singulet::Verbose = true;


    Libc::IOBuffer buff;

}
Y_UDONE()

