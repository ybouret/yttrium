
#include "y/concurrent/mutex/qnode.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        QMutex:: QMutex(const char * const id) noexcept :
        Quantized(),
        Mutex(id),
        next(0),
        prev(0)
        {

        }

        QMutex:: ~QMutex() noexcept
        {
            
        }
    }
}
