
#include "y/concurrent/mutex/qpool.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        QMutexPool:: QMutexPool() noexcept :
        Proxy<const QMutexPoolType>(),
        Identifiable(),
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
            return (my.size > 0) ? my.query() : new QMutex( callSign() );
        }


    }

}
