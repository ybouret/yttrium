

#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Solver:: Solver(const Cluster &cl) :
        Joint(cl),
        afm(),
        ceq(neqs,nspc),
        pps(neqs),
        dof(mine.Nu.rows),
        obj(neqs),
        ortho(nspc,dof),
        pbank(),
        basis(pbank),
        Cin(nspc),
        Cex(nspc),
        Cws(nspc),
        ddC(nspc),
        inc(nspc),
        xlu(dof),
        xsf(0.99)
        {
        }


        Solver:: ~Solver() noexcept {}

        void Solver:: showProspects(XMLog &xml, const XReadable &Ktop) const
        {
            if(xml.verbose)
            {
                for(size_t i=1;i<=pps.size();++i)
                {
                    pps[i].show( xml(), mine, Ktop) << std::endl;
                }
            }
        }

        bool Solver:: basisOkWith(const XReadable &C, const Level L) const noexcept
        {
            for(const PNode *pn=basis.head;pn;pn=pn->next)
            {
                if( ! (**pn).eq.canTolerate(C,L) ) return false;
            }
            return true;
        }
        
        bool Solver:: mustCut(xreal_t         &scale,
                              const XReadable &C,
                              const XReadable &dC) const noexcept
        {

            scale      = 1.0;
            bool abate = false;

            assert( basisOkWith(C,SubLevel) );

            for(size_t j=C.size();j>0;--j)
            {
                const xreal_t d = dC[j];  if(d.mantissa>=0) continue;
                const xreal_t c = C[j];   assert(c.mantissa>=0);
                const xreal_t f = c/(-d);
                if(f<=scale)
                {
                    abate = true;
                    scale = f;
                }
            }

            if( abate )
            {
                scale *= xsf;
            }

            return abate;
        }

    }

}
