//! \file

#ifndef Y_Apex_Types_Included
#define Y_Apex_Types_Included 1

#include "y/calculus/align.hpp"
#include "y/concurrent/mutex/qnode.hpp"


namespace Yttrium
{
    namespace Apex
    {

        typedef uint64_t  natural_t; //!< alias
        typedef int64_t   integer_t; //!< alias

        //! how data if planned
        enum Plan
        {
            Plan1=0, //!< uint8_t
            Plan2=1, //!< uint16_t
            Plan4=2, //!< uint32_t
            Plan8=3  //!< uint64_t
        };

        //! helper to convert bits to aligned byte(s)
#define Y_Apex_Bytes_For(BITS) ( (Y_ALIGN_ON(8,BITS)) >> 3 )

        typedef Concurrent::Mutex  MutexType;
        typedef Concurrent::QMutex Mutex;


    }
}

#endif
