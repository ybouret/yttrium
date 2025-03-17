//! \file


#ifndef Y_Chemical_Defs_Included
#define Y_Chemical_Defs_Included 1

#include "y/mkl/xreal.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;  //!< alias
        typedef XReal<real_t>               xreal_t; //!< alias
        typedef MKL::Antelope::Add<xreal_t> XAdd;    //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul;    //!< alias

        typedef Readable<xreal_t>      XReadable; //!< alias
        typedef Writable<xreal_t>      XWritable; //!< alias
        typedef Matrix<int>            iMatrix;   //!< alias
        typedef Matrix<unsigned>       uMatrix;   //!< alias
        typedef Matrix<xreal_t>        XMatrix;   //!< alias

    }

}

#endif

