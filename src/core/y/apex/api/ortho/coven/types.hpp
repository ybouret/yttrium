//! \file

#ifndef Y_Apex_Ortho_Coven_Types_Included
#define Y_Apex_Ortho_Coven_Types_Included 1


#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                typedef Small::CoopHeavyList<size_t> IList;
                typedef IList::NodeType              INode;
                typedef IList::ProxyType             IBank;
                
            }
        }
    }
}

#endif

