//! \file

#ifndef Y_Concurrent_Task_Flag_Included
#define Y_Concurrent_Task_Flag_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //! status of the task
        enum TaskFlag
        {
            TaskSuccess, //!< done
            TaskPending, //!< pending
            TaskRunning  //!< running
        };

    }

}

#endif
