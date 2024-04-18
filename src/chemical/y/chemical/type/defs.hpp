//! \file

#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/api.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double        Real;   //!< hardware real
        typedef XReal<double> XReal;  //!< software real
    }
}

#endif
