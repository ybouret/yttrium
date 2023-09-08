//! \file

#ifndef Y_Type_Temporary_Included
#define Y_Type_Temporary_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! assigning temporary values
    //
    //__________________________________________________________________________
    template <typename T>
    class Temporary
    {
    public:

        Y_ARGS_DECL(T,Type); //!< aliases

        //! saving old value, assigning new one, assuming no exception
        inline Temporary(Type &target, ParamType value) noexcept :
        host(target), save(target) { target  = value; }

        //! restore saved value
        inline ~Temporary() noexcept { host = save; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Temporary);
        Type      &host;
        ConstType  save;
    };
}



#endif
