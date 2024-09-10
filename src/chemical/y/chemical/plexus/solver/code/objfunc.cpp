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
            for(size_t j=nspc;j>0;--j)
            {
                const xreal_t c0 = Cin[j];
                const xreal_t c1 = Cex[j];
                xreal_t cmin=c0, cmax=c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Cws[j] = Clamp(cmin, c0*v + c1*u, cmax);
            }
            return Cws;
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


        xreal_t Solver:: objGrad(const XReadable &C,
                                 const Level      L)
        {
            const xreal_t _0;
            inc.forEach( &XAdd::free );
            obj.free();
            grd.ld(_0);

            const size_t m  = nspc;
            XWritable &  dA = Cws;

            for(size_t i=pps.size();i>0;--i)
            {
                const Prospect    &pro = pps[i];
                const Equilibrium &eq  = pro.eq;
                const xreal_t      A   = pro.affinity(afm.xmul,C,L);
                eq.drvsAffinity(dA, SubLevel, C, L);
                for(size_t j=m;j>0;--j)
                {
                    inc[j] << A * dA[j];
                }
                obj << A;
            }

            const xreal_t den = afm.xadd.normOf(obj);
            if(den.mantissa>0)
            {
                for(size_t j=m;j>0;--j)
                {
                    grd[j] = inc[j].sum() / den;
                }
            }

            return den;
        }

    }

}


