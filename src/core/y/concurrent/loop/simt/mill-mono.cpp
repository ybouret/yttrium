
#include "y/concurrent/loop/simt/mill.hpp"
#include "y/concurrent/loop/mono.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        
        SIMT_Mill:: SIMT_Mill(const Sequential_ &) :
        mill( new Mono() )
        {
        }

    }

}

