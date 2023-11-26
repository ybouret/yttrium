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

            Task::ID     push(const Task &task);
            virtual void flush() noexcept = 0;



        protected:
            explicit Pipeline() noexcept; //!< setup
            void     upgrade()  noexcept; //!< jobID++, controlled

            const Task::ID tid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);

            virtual void     suspend() noexcept          = 0; //!< suspend mechanism
            virtual void     restart() noexcept          = 0; //!< restart mechanism
            virtual Task::ID enqueue(const Task &, const Task::ID) = 0;
        };

    }

}

#endif

