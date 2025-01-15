
//! \file

#ifndef Yttrium_Concurrent_Mutex_QNode_Included
#define Yttrium_Concurrent_Mutex_QNode_Included 1


#include "y/concurrent/mutex.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class QMutex : public Quantized, public Mutex
        {
        public:
            explicit QMutex(const char * const=0) noexcept;
            virtual ~QMutex() noexcept;
            QMutex *next;
            QMutex *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QMutex);
        };

    }

}

#endif


