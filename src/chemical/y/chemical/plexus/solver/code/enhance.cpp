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
            const xreal_t sig = afm.xadd.dot(pro.dc,grd); // numeric slope
            if(sig.mantissa>=0.0)
            {
                //------------------------------------------------------
                // positive or zero slope, cancel this position
                //------------------------------------------------------
                pro.cc.ld(Cin);
                pro.dc.ld(pro.ax=pro.xi=0);
                pro.ff = ff0;
                return (pro.ok = false);
            }
            else
            {
                //------------------------------------------------------
                // initialize end point and triplets
                //------------------------------------------------------
                {
                    Cex.ld(pro.cc);
                    Triplet<xreal_t> uu   = { 0,   -1, 1      };
                    Triplet<xreal_t> ff   = { ff0, -1, pro.ff };
                    pro.ff = fcn( Minimize<xreal_t>::Locate(Minimizing::Inside,fcn,uu,ff) );
                 }

                //------------------------------------------------------
                // update status: cc, then dc, xi and |xi|
                //------------------------------------------------------
                pro.cc.ld(Cws);
                pro.ax = (pro.xi = afm.eval(pro.dc,pro.cc,SubLevel,Cin,SubLevel,pro.eq)).abs();
                return (pro.ok = true);
            }

        }
    }

}
