//! \file

#ifndef Y_Type_Releasable_Included
#define Y_Type_Releasable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Releasable interface
    //
    //
    //__________________________________________________________________________
    class Releasable
    {

    protected:
        explicit     Releasable() noexcept;        //!< setup
    public:
        virtual     ~Releasable() noexcept;        //!< cleanup
        virtual void release()    noexcept  = 0;   //!< release all possible

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Releasable);
    };

}

#endif
