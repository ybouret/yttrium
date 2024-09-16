#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        void Solver:: computeRate(XWritable &rate)
        {
            
            //------------------------------------------------------------------
            //
            // initialize inc
            //
            //------------------------------------------------------------------
            inc.forEach( &XAdd::free );

            //------------------------------------------------------------------
            //
            // use prospects to compute increases
            //
            //------------------------------------------------------------------
            for(size_t i=pps.size();i>0;--i)
            {
                const Prospect &pro = pps[i];
                if(!pro.ok) continue; //
                pro.step(inc);
            }

            //------------------------------------------------------------------
            //
            // deduce rate
            //
            //------------------------------------------------------------------
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = inc[j].sum();
            }

        }



        bool Solver:: odeStep(XMLog &xml)
        {
            //------------------------------------------------------------------
            //
            //
            // incoming with Cin, ff0, pps, basis, gradient, ff0
            //
            //
            //------------------------------------------------------------------


            const size_t n = pps.size();
            const size_t m = nspc;
            Y_XML_SECTION_OPT(xml, ODE_Step, " n='" << n << "' m='" << m << "'");

            computeRate(ddC);

            {
                xreal_t scale;
                if( stepWasCut(Cex,Cin,ddC,&scale))
                {
                    Y_XMLOG(xml, "# scale = " << real_t(scale));
                }
                else
                {
                    Y_XMLOG(xml, "# full step");
                }
            }

            const xreal_t slope = afm.xadd.dot(grd,ddC);
            Y_XMLOG(xml, "slope = " << real_t(slope));
            if(slope.mantissa>=0)
            {
                Y_XMLOG(xml, "positive slope");
                return false;
            }
            
            return located(ODE_Step,xml);
        }



    }
}
