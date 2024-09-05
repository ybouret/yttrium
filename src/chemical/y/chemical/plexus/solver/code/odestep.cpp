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

            // collect of increases from current basis
            for(const PNode *pn=basis.head;pn;pn=pn->next)
                (**pn).step(inc);

            // deduce rate
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = inc[j].sum();
            }

        }



        void Solver:: odeStep(XWritable &C, const Level L, XMLog &xml)
        {
            const size_t n = basis.size;
            const size_t m = nspc;

            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            // dC = Nu' * xi
            xreal_t full   = 1;
            xreal_t half   = 0.5;
            mine.transfer(Cin, SubLevel, C, L);
            computeRate(ddC);
            Y_XMLOG(xml, "ddC=" << ddC );

            {
                xreal_t scale;
                if(stepWasCut(Cex, Cin, ddC, &scale))
                {
                    Y_XMLOG(xml, "cut by " << real_t(scale));
                    full *= scale;
                    half *= scale;
                }
            }


            Solver &F = *this;
            std::cerr << "Ain = " << real_t(objFunc(Cin, SubLevel)) << " / " << real_t(F(0)) << std::endl;
            std::cerr << "Aex = " << real_t(objFunc(Cex, SubLevel)) << " / " << real_t(F(1)) << std::endl;
            {
                OutputFile fp("odestep.dat");
                const size_t np = 1000;
                for(size_t i=0;i<=np;++i)
                {
                    const real_t u = double(i)/np;
                    fp("%.15g %.15g\n", u, real_t(F(u)));
                }
            }
        }


    }
}
