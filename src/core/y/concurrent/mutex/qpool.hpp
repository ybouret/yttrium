
//! \file

#ifndef Yttrium_Concurrent_Mutex_QPool_Included
#define Yttrium_Concurrent_Mutex_QPool_Included 1


#include "y/concurrent/mutex/qnode.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/type/proxy.hpp"
#include "y/type/identifiable.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef CxxPoolOf<QMutex> QMutexPoolType;

        class QMutexPool : public Proxy<const QMutexPoolType>, public Identifiable
        {
        protected:
            explicit QMutexPool() noexcept;

        public:
            virtual ~QMutexPool() noexcept;

            QMutex * query();
            void     store(QMutex * const) noexcept;
            void     gc()                   noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QMutexPool);
            Y_PROXY_DECL();
            QMutexPoolType my;

        };

    }

}

#endif

