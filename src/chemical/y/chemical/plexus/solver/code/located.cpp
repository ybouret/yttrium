#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        bool Solver:: located(const char * const fn, XMLog &xml)
        {
            assert(0!=fn);
            Triplet<xreal_t> uu = { 0,   -1, 1 };
            Triplet<xreal_t> ff = { ff0, -1, objFunc(Cex,SubLevel) };
            const xreal_t    uu1 = Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff);
            const xreal_t    ff1 = fcn(uu1);

            Y_XMLOG(xml, "ff0   = " << real_t(ff0));
            Y_XMLOG(xml, "ff1   = " << real_t(ff1) <<  " @" << real_t(uu1));

            if(ff1<=ff0)
            {
                vlist.push(Cws,ff1).info  = fn;
                saveProfile(fn,1000);
                return true;
            }
            else
                return false;
        }
    }
    
}