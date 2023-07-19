
#include "y/io/cache.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/concurrent/singulet.hpp"

using namespace Yttrium;

Y_UTEST(io_char)
{
    Concurrent::Singulet::Verbose = true;

    IO::Cache cache;
    cache << "Hello, World!";
    cache << '-';
    cache >> '+';
    std::cerr << cache << std::endl;
    {
        const IO::Cache cpy(cache);
        std::cerr << cpy << std::endl;
    }
}
Y_UDONE()

