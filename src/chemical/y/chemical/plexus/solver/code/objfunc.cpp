#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Solver:: operator()(const xreal_t u)
        {
            return objFunc( probe(u), SubLevel);
        }

        const XReadable & Solver:: probe(const xreal_t u)
        {
            const xreal_t one = 1;
            const xreal_t v   = one-u;
            XWritable    &C   = Cws;
            for(size_t j=nspc;j>0;--j)
            {
                const xreal_t c0 = Cin[j];
                const xreal_t c1 = Cex[j];
                xreal_t cmin=c0, cmax=c1;
                if(cmax<cmin) Swap(cmin,cmax);
                C[j] = Clamp(cmin, c0*v + c1*u, cmax);
            }
            return C;
        }

        xreal_t Solver:: objFunc(const XReadable &C, const Level L)
        {
            obj.free();

            for(size_t i=pps.size();i>0;--i)
            {
                obj << pps[i].affinity(afm.xmul,C,L);
            }

            return afm.xadd.normOf(obj);
        }
    }

}


