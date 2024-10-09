#include "y/chemical/plexus/device.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        bool Device:: nullify(Ansatz &ans) noexcept
        {
            ans.dc.ld(0);
            ans.cc.ld(Cini);
            ans.ff = ff0;
            ans.xi = ans.ax = 0;
            return (ans.ok = false);
        }

        using namespace MKL;

        xreal_t Device:: lookUp()
        {
            Device          &F   = *this;
            Triplet<xreal_t> xx  = { 0,   -1, 1 };
            Triplet<xreal_t> ff  = { ff0, -1,      objectiveFunction(Cend,SubLevel) };
            const xreal_t    xm  = Minimize<xreal_t>::Locate(Minimizing::Inside, F, xx, ff);
            return F(xm);
        }

        
        bool Device:: enhance(Ansatz &ans)
        {
            const xreal_t slope = aftermath.xadd.dot(ans.dc,gradient);
            if(slope.mantissa>=0.0)
            {
                //--------------------------------------------------------------
                //
                // numerically not satistfying
                //
                //--------------------------------------------------------------
                return nullify(ans);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // look for mininimum in [Cini:ans.cc]
                //
                //--------------------------------------------------------------
                Cend.ld(ans.cc);
                Device          &F  = *this;
                Triplet<xreal_t> xx = { 0,   -1,      1 };
                Triplet<xreal_t> ff = { ff0, -1, ans.ff };
                const xreal_t    xm = Minimize<xreal_t>::Locate(Minimizing::Inside, F, xx, ff);

                //--------------------------------------------------------------
                //
                // recompute ansatz from the minimum
                //
                //--------------------------------------------------------------
                ans.ff = F(xm); if(ans.ff>=ff0) return nullify(ans);
                ans.cc.ld(Ctmp);
                ans.xi = aftermath.eval(ans.dc, ans.cc, SubLevel, Cini, SubLevel,ans.eq);
                ans.ax = ans.xi.abs();
                return (ans.ok = true);
            }
        }
    }
}


