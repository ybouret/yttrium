
//! \file

#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/api.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/cxx/array.hpp"
#include "y/container/cxx/series.hpp"
#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //----------------------------------------------------------------------
        typedef double                      real_t;    //!< machine real
        typedef XReal<real_t>               xreal_t;   //!< soft real

        //----------------------------------------------------------------------
        typedef Readable<xreal_t>           XReadable; //!< alias
        typedef Writable<xreal_t>           XWritable; //!< alias
        typedef Memory::Dyadic              XMemory;   //!< alias
        typedef Matrix<xreal_t,XMemory>     XMatrix;   //!< alias
        typedef Vector<xreal_t,XMemory>     XVector;   //!< alias
        typedef CxxArray<xreal_t,XMemory>   XArray;    //!< alias
        typedef CxxSeries<xreal_t,XMemory>  XSeries;   //!< alias
        typedef CxxSeries<XMatrix>          XMatrices; //!< alias

        //----------------------------------------------------------------------
        typedef MKL::Antelope::Add<xreal_t> XAdd;   //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul;   //!< alias
        typedef CxxArray<XAdd,XMemory>      XSwell; //!< alias
    
        //----------------------------------------------------------------------
        typedef LittleEndianKey             KeyType; //!< alias
        typedef LittleEndianAddress         KeyAddr; //!< alias
    }
}

#endif

