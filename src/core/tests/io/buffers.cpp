

#include "y/io/libc/buffer/direct.hpp"
#include "y/io/libc/buffer/cached.hpp"

#include "y/utest/run.hpp"
#include "y/concurrent/singulet.hpp"

using namespace Yttrium;

Y_UTEST(io_buffers)
{
    Concurrent::Singulet::Verbose = true;


    Libc::DirectBuffer dbuf;
    Libc::CachedBuffer cbuf;


}
Y_UDONE()

