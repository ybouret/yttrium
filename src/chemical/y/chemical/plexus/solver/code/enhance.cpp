#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;


        bool Solver:: enhance(Prospect &pro)
        {
            //------------------------------------------------------------------
            // assuming Cin and ff0 are set
            //------------------------------------------------------------------
            const xreal_t sig = afm.xadd.dot(pro.dc,grd); // slope
            if(sig.mantissa>=0.0)
            {
                //------------------------------------------------------
                // positive or zero slope, cancel this position
                //------------------------------------------------------
                pro.cc.ld(Cin);
                pro.dc.ld(pro.ax=pro.xi=0);
                pro.ff = ff0;
                return false;
            }
            else
            {
                //------------------------------------------------------
                // initialize end point and triplets
                //------------------------------------------------------
                Cex.ld(pro.cc);
                Triplet<xreal_t> uu   = { 0,   -1, 1      };
                Triplet<xreal_t> ff   = { ff0, -1, pro.ff };

                //------------------------------------------------------
                // update status: cc and xi
                //------------------------------------------------------
                pro.ff = fcn( Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff) );
                pro.cc.ld(Cws);
                pro.ax = (pro.xi = afm.eval(pro.dc, pro.cc, SubLevel, Cin, SubLevel, pro.eq)).abs();
                return true;
            }

        }
    }

}
