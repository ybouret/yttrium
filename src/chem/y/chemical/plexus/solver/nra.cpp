
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

            //------------------------------------------------------------------
            //
            //
            // extracting local basis
            //
            //
            //------------------------------------------------------------------
            {
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

            //------------------------------------------------------------------
            //
            //
            // computing local algebra
            //
            //
            //------------------------------------------------------------------
            const size_t n = basis.size;
            const size_t m = mix->species.size;
            XArray       xi(n);
            XMatrix      Phi(n,m);
            XMatrix      Nu(n,m);
            XMatrix      NuT(m,n);
            XMatrix      Xi(n,n);

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

            // preparing system
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    Xi[i][j] = xadd.dot(Phi[i], Nu[j]);
                }
            }

            Y_XMLOG(xml,"Xi="<<Xi);

            if( !lu.build(Xi) )
            {
                Y_XML_COMMENT(xml, "singular system");
                return f0;
            }

            lu.solve(Xi,xi);
            Y_XMLOG(xml, "xi=" << xi);
            for(size_t j=m;j>0;--j)
                step[j] = xadd.dot(NuT[j],xi);

            Y_XMLOG(xml, "step=" << step);



            throw Exception("not implemented");
            return f0;
        }

        
    }

}
