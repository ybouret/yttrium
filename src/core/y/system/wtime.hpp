//! \file

#ifndef Y_System_WallTime_Included
#define Y_System_WallTime_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    class WallTime
    {
    public:
        explicit WallTime();
        virtual ~WallTime() noexcept;

        static uint64_t Ticks();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(WallTime);
    };

}

#endif

