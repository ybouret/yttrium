
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chem/plexus/conservation.hpp"
#include "y/data/small/light/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::BareLightList<const Conservation> Conservations;
        
    }

}

#endif

