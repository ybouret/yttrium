//! \file

#ifndef Y_Chemical_Defs_Included
#define Y_Chemical_Defs_Included 1

#include "y/chemical/type/memory.hpp"
#include "y/mkl/xreal.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double          real_t;    //!< alias
        typedef XReal<real_t>   xReal;     //!< alias
        typedef Writable<xReal> XWritable; //!< alias
        typedef Readable<xReal> XReadable; //!< alias


        typedef Vector<String,MemoryModel> Strings; //!< alias
    }

}

#endif
