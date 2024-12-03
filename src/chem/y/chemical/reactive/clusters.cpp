
#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        Clusters:: ConstInterface & Clusters:: surrogate() const noexcept
        {
            return cls;
        }

        Clusters:: Clusters(const Equilibria &eqs,
                            XMLog            &xml) :
        Proxy<const Cluster::List>(),
        cls()
        {
            Y_XML_SECTION(xml, "Clusters");
            
        }

    }
    
}
