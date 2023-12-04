//! \file

#ifndef Y_Concurrent_Pipeline_Included
#define Y_Concurrent_Pipeline_Included 1

#include "y/concurrent/pipeline/task.hpp"
#include "y/concurrent/thread/contexts.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! Interface to a Pipeline to process jobs
        //
        //
        //______________________________________________________________________
        class Pipeline : public ThreadContexts
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void flush() noexcept = 0; //!< wait for all jobs to complete
            void         reset() noexcept;     //!< reset tid

            //! push ONE task, retrieve its ID
            Task::ID     push(const Task &task);

            //! push a range of tasks, append their IDs
            template < typename SEQUENCE, typename ITERATOR>
            inline void push(SEQUENCE &tids,
                             ITERATOR  curr,
                             size_t    ntsk)
            {
                try {
                    suspend();
                    while(ntsk-- > 0)
                    {
                        const Task::ID tmp = enqueue(*curr,tid);
                        tids.pushTail(tmp);
                        upgrade();
                        ++curr;
                    }
                    restart();
                }
                catch(...)
                {
                    restart();
                    throw;
                }
            }

            //! push a full sequence of tasks, append their IDs
            template <typename SEQUENCE, typename TASKS>
            inline void push(SEQUENCE &tids, TASKS tasks)
            {
                push(tids,tasks.begin(),tasks.size());
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Pipeline() noexcept; //!< cleanup
        protected:
            explicit Pipeline() noexcept; //!< setup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);
            const Task::ID   tid;                                       //!< auto-incremented counter
            void             upgrade() noexcept;                        //!< tid++, controlled
            virtual void     suspend() noexcept                    = 0; //!< suspend mechanism
            virtual void     restart() noexcept                    = 0; //!< restart mechanism
            virtual Task::ID enqueue(const Task &, const Task::ID) = 0; //!< enqueue a single task in a LOCKED pipeline
        };

        //______________________________________________________________________
        //
        //! alias to share Pipeline in Engines
        //______________________________________________________________________
        typedef ArcPtr<Pipeline> SharedPipeline;

    }

}

#endif

