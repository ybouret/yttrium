//! \file

#ifndef Y_Type_AutoClean_Included
#define Y_Type_AutoClean_Included 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"
#include <iostream>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! calling method of an host before return
    /**
     - simplify code in case of exception
     - the size of a pointer-to-member-function can change depending on the class!
     */
    //
    //
    //__________________________________________________________________________
    template <typename HOST>
    class AutoClean
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef HOST  Host;                //!< alias
        typedef void (Host::*Meth)(void);  //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from persitent host and its method
        inline explicit AutoClean(Host &h, Meth m) noexcept :
        host(h),
        meth(m)
        {
            assert(0!=meth);
        }

        //! cleanup
        inline virtual ~AutoClean() noexcept
        {
            try { (host.*meth)(); } catch(...) {}
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(AutoClean);
        Host &host;
        Meth  meth;
    };


}

#endif

