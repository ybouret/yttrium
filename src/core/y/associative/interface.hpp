//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Associative interface
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class Associative
    {
    protected:
        inline explicit Associative() noexcept {}
    public:
        inline virtual ~Associative() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Associative);
    };

}

#endif

