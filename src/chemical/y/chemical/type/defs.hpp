//! \file


#ifndef Y_Chemical_Defs_Included
#define Y_Chemical_Defs_Included 1

#include "y/mkl/xreal.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/triplet.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;  //!< alias
        typedef XReal<real_t>               xreal_t; //!< alias
        typedef MKL::Antelope::Add<xreal_t> XAdd;    //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul;    //!< alias
        typedef Memory::Dyadic              MemoryModel; //!< alias

        typedef Readable<xreal_t>             XReadable; //!< alias
        typedef Writable<xreal_t>             XWritable; //!< alias
        typedef Matrix<int,MemoryModel>       iMatrix;   //!< alias
        typedef Matrix<unsigned,MemoryModel>  uMatrix;   //!< alias
        typedef Matrix<xreal_t,MemoryModel>   XMatrix;   //!< alias
        typedef CxxArray<xreal_t,MemoryModel> XArray;    //!< alias

        typedef MKL::Triplet<xreal_t>        XTriplet; //!< alias

    }

}

#endif

