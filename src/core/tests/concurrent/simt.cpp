#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frames.hpp"
#include "y/utest/run.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        template <typename ENGINE>
        class SIMT : public Frames<ENGINE>
        {
        };
    }
}

using namespace Yttrium;


Y_UTEST(concurrent_simt)
{
    
}
Y_UDONE()

