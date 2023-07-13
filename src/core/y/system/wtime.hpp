//! \file

#ifndef Y_System_WallTime_Included
#define Y_System_WallTime_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! real time handling
    //
    //
    //__________________________________________________________________________
    class WallTime
    {
    public:
        explicit WallTime();           //!< setup
        virtual ~WallTime() noexcept;  //!< cleanup

        static uint64_t Ticks();       //!< system's ticks

        const double freq;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(WallTime);
    };

}

#endif

