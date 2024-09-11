#include "y/chemical/plexus/solver.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        void Solver:: computeRate(XWritable &rate)
        {
            
            // initialize inc
            inc.forEach( &XAdd::free );


            for(size_t j=pps.size();j>0;--j)
                pps[j].step(inc);

            // deduce rate
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = inc[j].sum();
            }

        }



        bool Solver:: odeStep( XMLog &xml)
        {
            const size_t n = pps.size();
            const size_t m = nspc;
            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            // incoming with Cin, ff0, pps, basis, gradient, ff0
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
                return false;
            }

            Triplet<xreal_t> uu = { 0,   -1, 1 };
            Triplet<xreal_t> ff = { ff0, -1, objFunc(Cex,SubLevel) };
            const xreal_t    uu1 = Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff);
            const xreal_t    ff1 = fcn(uu1);

            Y_XMLOG(xml, "ff0   = " << real_t(ff0));
            Y_XMLOG(xml, "ff1   = " << real_t(ff1) <<  " @" << real_t(uu1));

            saveProfile("odestep.dat",1000);

            return false;

        }


    }
}
