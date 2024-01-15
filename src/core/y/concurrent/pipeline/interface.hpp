//! \file

#ifndef Y_Concurrent_Pipeline_Included
#define Y_Concurrent_Pipeline_Included 1

#include "y/concurrent/pipeline/task/manager.hpp"
#include "y/concurrent/pipeline/task/flag.hpp"
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
        class Pipeline : public ThreadContexts, public TaskManager
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void     flush()                     noexcept = 0; //!< wait for all jobs to complete
            virtual TaskFlag query(const TaskUUID) const noexcept = 0; //!< query task status

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void                 reset()                     noexcept;     //!< reset internal tid
            virtual Lockable    &sync()                      noexcept = 0; //!< get synchronization lock
            virtual TaskUUID     load(const Task &task);                   //!< push ONE task, retrieve its ID

            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________

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
                        const TaskUUID tmp = enqueue(*curr,tid);
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
            { push(tids,tasks.begin(),tasks.size()); }


            
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
            const TaskUUID   tid;                                       //!< auto-incremented counter
            void             upgrade() noexcept;                        //!< tid++, controlled
            virtual void     suspend() noexcept                    = 0; //!< suspend mechanism
            virtual void     restart() noexcept                    = 0; //!< restart mechanism
            virtual TaskUUID enqueue(const Task &, const TaskUUID) = 0; //!< enqueue a single task in a LOCKED pipeline
        };

        //______________________________________________________________________
        //
        //! alias to share Pipeline in Engines
        //______________________________________________________________________
        typedef ArcPtr<Pipeline> SharedPipeline;

    }

}

#endif

