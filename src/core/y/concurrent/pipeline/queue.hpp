

//! \file

#ifndef Y_Concurrent_Queue_Included
#define Y_Concurrent_Queue_Included 1

#include "y/concurrent/pipeline/interface.hpp"
#include "y/concurrent/topology.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Interface to a multi-threaded pipeline
        //
        //
        //______________________________________________________________________
        class Queue : public Pipeline
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Concurrent::Queue
            class Code;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Queue(const Topology &); //!< setup from topology
            virtual ~Queue() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual size_t       size()                   const noexcept; //!< 1
            virtual ConstType &  operator[](const size_t) const noexcept; //!< self

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Queue);
            Code *code;
        };
    }

}

#endif

