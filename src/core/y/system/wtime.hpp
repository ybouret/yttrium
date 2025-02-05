//! \file

#ifndef Y_System_WallTime_Included
#define Y_System_WallTime_Included 1

#include "y/type/moniker.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Wall Time handling
    //
    //
    //__________________________________________________________________________
    class WallTime
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit WallTime();           //!< setup
        virtual ~WallTime() noexcept;  //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        static uint64_t Ticks();                                   //!< system's ticks
        static uint64_t LockedTicks();                             //!< with locking
        long double     operator()(const uint64_t) const noexcept; //!< ticks to seconds
        void            wait(const double nsec)             const; //!< self loop
        void            lockedWait(const double nsec)       const; //!< self loop
        long double     since(const uint64_t)               const; //!< ellapsed since mark

        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const Moniker<long double> freq; //!< system ticks to time factor


    private:
        Y_DISABLE_COPY_AND_ASSIGN(WallTime);
    };

}

#endif

