
#include "y/chemical/plexus/solver.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xReal Solver:: buildNRA(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml,NRA);

            EBank ebank;
            ERepo basis(ebank);

            // extracting
            family.free();
            const size_t dof = mix->topology.rows;
            for(const ProNode *pn=my.head;pn;pn=pn->next)
            {
                const Components    &eq = (**pn).eq;
                const Readable<int> &nu = mix.topology[ eq.indx[SubLevel] ];
                if( family.wouldAccept(nu) )
                {
                    family.expand();
                    //basis << eq;
                    if( basis.size >= dof) break;
                }
            }

            throw Exception("not implemented");
            return f0;
        }

        
    }

}
