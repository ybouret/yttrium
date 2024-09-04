

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


    }

}
