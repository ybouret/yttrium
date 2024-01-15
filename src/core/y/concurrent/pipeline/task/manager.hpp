
//! \file

#ifndef Y_Concurrent_Task_Manager_Included
#define Y_Concurrent_Task_Manager_Included 1

#include "y/concurrent/pipeline/task.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base class for Pipeline
        //
        //
        //______________________________________________________________________
        class TaskManager
        {
        public:

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void load(const Task &task);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~TaskManager() noexcept;

        protected:
            explicit TaskManager() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TaskManager);
        };


    }

}

#endif
