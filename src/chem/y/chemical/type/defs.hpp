//! \file

#ifndef Y_Chemical_Defs_Included
#define Y_Chemical_Defs_Included 1

#include "y/chemical/type/memory.hpp"
#include "y/mkl/xreal.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/mkl/triplet.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double          real_t;    //!< alias
        typedef XReal<real_t>   xReal;     //!< alias
        typedef Writable<xReal> XWritable; //!< alias
        typedef Readable<xReal> XReadable; //!< alias

        typedef MKL::Antelope::Add<xReal> XAdd;     //!< alias
        typedef MKL::Antelope::Mul<xReal> XMul;     //!< alias
        typedef MKL::Triplet<xReal>       XTriplet; //!< alias

        typedef Vector<String,MemoryModel> Strings; //!< alias
        typedef Vector<xReal,MemoryModel>  XVector; //!< alias


    }

}

#endif
