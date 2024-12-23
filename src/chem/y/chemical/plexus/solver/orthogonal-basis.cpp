#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Solver:: getOrthoBasis(XMLog &xml)
        {
            Y_XML_SECTION(xml, "OrthogonalBasis");
            ortho.free();
            basis.free();
            const size_t dof = mix->topology.rows;
            for(ProNode *pn=my.head;pn;pn=pn->next)
            {
                Prospect            &pro = **pn;
                const Components    &eq = pro.eq;
                const Readable<int> &nu = mix.topology[ eq.indx[SubLevel] ];
                if( ortho.wouldAccept(nu) )
                {
                    ortho.expand();
                    basis << pro;
                    assert(basis.size==ortho.size);
                    Y_XMLOG(xml, "(+) " << pro.key());
                    Coerce(pro.eq.indx[AuxLevel]) = basis.size;
                    if( basis.size >= dof) break;
                }
            }

            
        }

    }

}

