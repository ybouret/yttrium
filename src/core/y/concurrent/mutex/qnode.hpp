
//! \file

#ifndef Yttrium_Concurrent_Mutex_QNode_Included
#define Yttrium_Concurrent_Mutex_QNode_Included 1


#include "y/concurrent/mutex.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Quantized Dynamic Mutex
        //
        //
        //______________________________________________________________________
        class QMutex : public Quantized, public Mutex
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit QMutex(const char * const=0) noexcept; //!< setup
            virtual ~QMutex() noexcept;                     //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            QMutex *next; //!< for list/pool
            QMutex *prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(QMutex);
        };

    }

}

#endif


