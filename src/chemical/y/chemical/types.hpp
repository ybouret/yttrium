
//! \file

#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/api.hpp"
#include "y/mkl/antelope/caddy.hpp"
#include "y/mkl/antelope/mul.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;    //!< machine real
        typedef XReal<real_t>               xreal_t;   //!< soft real
        
        typedef Readable<xreal_t>           XReadable; //!< alias
        typedef Writable<xreal_t>           XWritable; //!< alias

        typedef MKL::Antelope::Add<xreal_t> XAdd; //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul; //!< alias
    }
}

#endif

