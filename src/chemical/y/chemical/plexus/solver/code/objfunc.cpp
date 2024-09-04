#include "y/chemical/plexus/solver.hpp"

namespace Yttrium
{
    namespace Chemical
    {




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


