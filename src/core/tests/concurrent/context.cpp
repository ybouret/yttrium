#include "y/concurrent/context.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(concurrent_context)
{

    size_t size = 12;
    for(size_t rank=0;rank<size;++rank)
    {
        Concurrent::Context ctx(size,rank);
        std::cerr << ctx.size << "." << ctx.rank << " => '" << ctx.name << "'" << std::endl;
    }

    Y_SIZEOF(Concurrent::Context);
    
}
Y_UDONE()

