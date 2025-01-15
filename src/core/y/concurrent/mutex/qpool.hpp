
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

        //______________________________________________________________________
        //
        //
        //! base type pour QMutexPool
        //
        //______________________________________________________________________
        typedef CxxPoolOf<QMutex> QMutexPoolType;

        //______________________________________________________________________
        //
        //
        //
        //! Pool of Quantized Mutexes
        //
        //
        //______________________________________________________________________
        class QMutexPool : public Proxy<const QMutexPoolType>, public Identifiable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit QMutexPool() noexcept; //!< setup

        public:
            virtual ~QMutexPool() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            QMutex * query();                          //!< query/create with callSign()
            void     store(QMutex * const)   noexcept; //!< store
            void     gc(const size_t cycles) noexcept; //!< GC

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QMutexPool);
            Y_PROXY_DECL();
            QMutexPoolType my;

        };

    }

}

#endif

