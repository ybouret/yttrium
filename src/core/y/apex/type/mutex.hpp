
//! \file

#ifndef Y_Apex_Mutex_Included
#define Y_Apex_Mutex_Included 1

#include "y/calculus/align.hpp"
#include "y/concurrent/mutex/qnode.hpp"


namespace Yttrium
{
    namespace Apex
    {
        typedef Concurrent::Mutex  MutexType; //!< alias
        typedef Concurrent::QMutex Mutex;     //!< alias
    }
}

#endif
