#include "y/chem/plexus/cluster.hpp"
#include "y/chem/algebraic.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: connectAll(XMLog &xml, Equilibria &all)
        {
            Y_XML_SECTION(xml, "ConnectAll");
            Chemical::Algebraic::Weight::List W;
            Chemical::Algebraic::Compute(W,Nu,xml);
        }
    }

}

