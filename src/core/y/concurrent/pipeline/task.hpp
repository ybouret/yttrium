//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/concurrent/pipeline/task/uuid.hpp"
#include "y/concurrent/pipeline/runnable.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate all different (ref-counted) Runnable for Pipeline
        //
        //
        //______________________________________________________________________
        class Task
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++ : allowing to make pre-compiled, re-usable tasks
            //
            //__________________________________________________________________
            explicit Task(Runnable   *) noexcept; //!< setup from user's runnable code
            Task(const Task &)          noexcept; //!< shared copy
            virtual ~Task()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

        protected:
            void initialize()  noexcept; //!< withold code

        private:
            Y_DISABLE_ASSIGN(Task);
            Runnable *code;
        };

        
    }

}

#endif
