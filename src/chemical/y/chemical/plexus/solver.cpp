

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
        xlu(dof)
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

    }

}
