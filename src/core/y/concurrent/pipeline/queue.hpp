//! \file

#ifndef Y_Concurrent_Queue_Included
#define Y_Concurrent_Queue_Included 1

#include "y/concurrent/pipeline/interface.hpp"
#include "y/concurrent/topology.hpp"
#include "y/concurrent/threads.hpp"

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
        class Queue : public Pipeline, public Threads
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
            virtual ~Queue() noexcept;        //!< cleanup, crushing unprocessed task(s)

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char *   callSign()                const noexcept; //!< CallSign
            virtual size_t         size()                    const noexcept; //!< number of threads
            virtual Lockable    &  sync()                          noexcept; //!< shared mutex
            virtual ConstType &    operator[](const size_t)  const noexcept; //!< self
            virtual void           flush()                         noexcept; //!< wait for jobs to complete.
            virtual TaskFlag       query(const TaskUUID)     const noexcept; //!< probe task id
            virtual size_t         cargo()                  const noexcept;  //!< internal jobs to do
            virtual const Thread & getThread(const size_t i) const noexcept; //!< getThread(1..size)
            virtual size_t         numThreads()              const noexcept; //!< size

            static Pipeline * Create(); //!< create with DefaultTopology

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Queue);
            Code *code;

            virtual void     suspend() noexcept;
            virtual void     restart() noexcept;
            virtual TaskUUID enqueue(const Task &task, const TaskUUID uuid);

        };
    }

}

#endif

