
#include "y/concurrent/mutex/qpool.hpp"
#include "y/data/gc/cxx-pool.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        QMutexPool:: QMutexPool(const char * const id) noexcept :
        Proxy<const QMutexPoolType>(),
        label(id),
        my()
        {
        }

        Y_PROXY_IMPL(QMutexPool,my)

        QMutexPool:: ~QMutexPool() noexcept
        {}

        void QMutexPool:: store(QMutex * const mtx) noexcept
        {
            assert(0!=mtx);
            my.store(mtx);
        }

        QMutex * QMutexPool:: query()
        {
            return (my.size > 0) ? my.query() : new QMutex(label);
        }

        void QMutexPool:: gc(const size_t cycles) noexcept
        {
            CxxPoolGC::Cycle(my,cycles);
        }


    }

}
