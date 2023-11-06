
//! \file

#ifndef Y_Thread_Proc_Included
#define Y_Thread_Proc_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef void (*ThreadProc)(void *); //!< generic parallel function
    }

}

#endif
