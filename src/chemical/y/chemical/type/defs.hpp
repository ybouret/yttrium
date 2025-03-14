//! \file


#ifndef Y_Chemical_Defs_Included
#define Y_Chemical_Defs_Included 1

#include "y/mkl/xreal.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/mul.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;  //!< alias
        typedef XReal<real_t>               xreal_t; //!< alias
        typedef MKL::Antelope::Add<xreal_t> XAdd;
        typedef MKL::Antelope::Mul<xreal_t> XMul;

    }

}

#endif

