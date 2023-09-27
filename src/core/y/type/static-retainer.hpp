//! \file

#ifndef Y_Static_Retainer_Included
#define Y_Static_Retainer_Included 1

#include "y/counted.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Helper to locally increase/decrease Counted references
    //
    //
    //__________________________________________________________________________
    class StaticRetainer
    {
    public:
        StaticRetainer(Counted &obj) noexcept; //!< withhold
        ~StaticRetainer()            noexcept; //!< liberate

    private:
        Y_DISABLE_COPY_AND_ASSIGN(StaticRetainer);
        Counted &host;
    };
}

#endif

