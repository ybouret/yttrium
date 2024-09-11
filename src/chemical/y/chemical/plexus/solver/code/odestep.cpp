#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Solver:: computeRate(XWritable &rate)
        {
            
            // initialize inc
            inc.forEach( &XAdd::free );


            for(size_t j=pps.size();j>0;--j)
            {
                pps[j].step(inc);
            }

            // deduce rate
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = inc[j].sum();
            }

        }



        void Solver:: odeStep(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            const size_t n = pps.size();
            const size_t m = nspc;
            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            // incoming with Cin, ff0, pps, basis and all dc
            computeRate(ddC);

            {
                xreal_t scale;
                if( stepWasCut(Cex,Cin,ddC,&scale))
                {
                    Y_XMLOG(xml, "cut @" << real_t(scale));
                }
            }

            std::cerr << "Ain = " << real_t(fcn(0)) << " / " << real_t(ff0) << std::endl;
            std::cerr << "Aex = " << real_t(fcn(1)) << std::endl;

            saveProfile("odestep.dat");



        }


    }
}
