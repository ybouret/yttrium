//! \file

#ifndef Y_NetworkSort_Tables_Included
#define Y_NetworkSort_Tables_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace NetworkSort
    {
        //______________________________________________________________________
        //
        //
        //! Table of indices
        //
        //______________________________________________________________________
        struct Table
        {
            const unsigned *index; //!< index[0..2*swaps]
            const unsigned  swaps; //!< number of swaps to test/perform
        };

        //______________________________________________________________________
        //
        //
        //! Database of tables
        //
        //______________________________________________________________________
        struct Tables
        {
            static const unsigned      MinSize = 2; //!< meaningless for 0 and 1
            static const unsigned      MaxSize;     //!< 32, could change
            static const Table * const CxxData;     //!< CxxData[MinSize..MaxSize]
        };




    }
}

#endif
