//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/task/callback.hpp"
#include "y/concurrent/pipeline/task/command.hpp"
#include "y/concurrent/pipeline/task/uuid.hpp"


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
            Task(const Task &) noexcept; //!< shared copy
            virtual ~Task()    noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

        protected:
            Task(Runnable   *) noexcept; //!< setup from user's runnable code
            void initialize()  noexcept; //!< withold code

        private:
            Y_DISABLE_ASSIGN(Task);
            Runnable *code;
        };

        
    }

}

#endif
