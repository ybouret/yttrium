
//! \file

#ifndef Y_Concurrent_Task_Manager_Included
#define Y_Concurrent_Task_Manager_Included 1

#include "y/concurrent/pipeline/task/uuid.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class Task;
        class Runnable;

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

            //! load a new pre-compiled task into manager
            virtual TaskUUID load(const Task &task) = 0;

            //! load a new one-time runnable into manager
            virtual TaskUUID run( Runnable * );

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
