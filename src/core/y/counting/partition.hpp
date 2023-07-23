//! \file

#ifndef Y_Counting_Partition_Included
#define Y_Counting_Partition_Included 1

#include "y/counting/schedule.hpp"
#include "y/apex/natural.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Partition of n
    //
    //
    //__________________________________________________________________________
    class Partition : public Schedule
    {
    public:
        static const char * const CallSign; //!< "Partition"
        static apn Cardinal(const size_t n);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Partition);
    };
    

}

#endif

