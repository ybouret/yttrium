
#include "y/concurrent/wire.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            WireData:: ~WireData() noexcept {
                Coerce(procAddr) = 0;
                Coerce(argsAddr) = 0;
            }

            void * WireData:: Args() noexcept { return this; }

        }

        Wire:: ~Wire() noexcept
        {
        }


    }

}


