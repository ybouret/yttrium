//! \file

#ifndef Y_Container_CxxCapacity_Included
#define Y_Container_CxxCapacity_Included 1

#include "y/memory/wad.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! selecting which capacity is desired
    //
    //__________________________________________________________________________
    enum CxxCapacity
    {
        CxxRequiredCapacity = 0, //!< user's request
        CxxAcquiredCapacity = 1  //!< from acquired memory
    };

    //__________________________________________________________________________
    //
    //
    //! template prototype for selectro
    //
    //__________________________________________________________________________
    template <int> struct CxxSetCapacity;

    //__________________________________________________________________________
    //
    //
    //! selecting user's required capacity
    //
    //__________________________________________________________________________
    template <> struct CxxSetCapacity<CxxRequiredCapacity>
    {
        //! choose n
        static size_t From(const size_t n, const Memory::Crux::Wad &) noexcept;
    };

    //__________________________________________________________________________
    //
    //
    //! selecting acquired memory capacity
    //
    //__________________________________________________________________________
    template <> struct CxxSetCapacity<CxxAcquiredCapacity>
    {
        //! choose w.capacity
        static size_t From(const size_t, const Memory::Crux::Wad &w) noexcept;
    };

}

#endif
