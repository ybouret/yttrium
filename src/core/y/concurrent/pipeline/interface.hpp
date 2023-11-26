//! \file

#ifndef Y_Concurrent_Interface_Included
#define Y_Concurrent_Interface_Included 1

#include "y/concurrent/pipeline/task.hpp"
#include "y/container/readable.hpp"


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
        class Pipeline : public Readable<const ThreadContext>
        {
        public:
            virtual ~Pipeline() noexcept; //!< cleanup

            virtual void flush() noexcept = 0;
            void         reset() noexcept; //!< reset tid

            Task::ID     push(const Task &task);
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

            template <typename SEQUENCE, typename TASKS>
            inline void push(SEQUENCE &tids, TASKS tasks)
            {
                push(tids,tasks.begin(),tasks.size());
            }


        protected:
            explicit Pipeline() noexcept; //!< setup

            const Task::ID tid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);
            void             upgrade() noexcept; //!< jobID++, controlled
            virtual void     suspend() noexcept                    = 0; //!< suspend mechanism
            virtual void     restart() noexcept                    = 0; //!< restart mechanism
            virtual Task::ID enqueue(const Task &, const Task::ID) = 0; //!< enqueue a single task in a LOCKED pipeline
            
        };

    }

}

#endif

