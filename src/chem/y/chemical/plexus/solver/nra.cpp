
#include "y/chemical/plexus/solver.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xReal Solver:: buildNRA(XMLog &xml, const xReal f0)
        {
            Y_XML_SECTION(xml,NRA);


            Y_XML_COMMENT(xml, "extracting basis");

            // extracting
            {
                family.free();
                basis.free();
                const size_t dof = mix->topology.rows;
                for(ProNode *pn=my.head;pn;pn=pn->next)
                {
                    Prospect            &pro = **pn;
                    const Components    &eq = pro.eq;
                    const Readable<int> &nu = mix.topology[ eq.indx[SubLevel] ];
                    if( family.wouldAccept(nu) )
                    {
                        family.expand();
                        basis << pro;
                        assert(basis.size==family.size);
                        Y_XMLOG(xml, "(+) " << pro.key());
                        Coerce(pro.eq.indx[AuxLevel]) = basis.size;
                        if( basis.size >= dof) break;
                    }
                }
            }

            const size_t n = basis.size;
            const size_t m = mix->species.size;
            XArray       xi(n);
            XMatrix      Phi(n,m);
            XMatrix      Nu(n,m);
            XMatrix      NuT(m,n);

            for(const ProNode *pn=basis.head;pn;pn=pn->next)
            {
                const Prospect   &   pro = **pn;
                const Components &   eq  = pro.eq;
                const size_t * const id  = eq.indx;
                const size_t         ei  = id[AuxLevel];

                xi[ei] = -eq.score(xmul, pro.eK, Cini, SubLevel);
                eq.jacobian(Phi[ei], SubLevel, Cini, SubLevel);
                Nu[ei].ld( mix.topology[ id[SubLevel] ] );
            }
            NuT.assign(TransposeOf,Nu);

            Y_XMLOG(xml, "rhs=" << xi);
            Y_XMLOG(xml, "Phi=" << Phi);
            Y_XMLOG(xml, "Nu="  << Nu);
            Y_XMLOG(xml, "NuT="  << NuT);



            throw Exception("not implemented");
            return f0;
        }

        
    }

}
