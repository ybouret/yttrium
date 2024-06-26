
//! \file

#ifndef Y_Concurrent_Task_UUID_Included
#define Y_Concurrent_Task_UUID_Included 1

#include "y/type/ints.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        typedef UnsignedInt<sizeof(void*)>::Type TaskUUID; //!< task identifier

    }

}

#endif
