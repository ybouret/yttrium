

#include "y/concurrent/loop/simt/mill.hpp"
#include "y/concurrent/loop/crew.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        SIMT_Mill:: SIMT_Mill(const InParallel_ &) :
        mill( Crew::Create() )
        {
        }

    }

}

