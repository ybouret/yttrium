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
            //const size_t n = basis.size;
            const size_t n = pps.size();
            const size_t m = nspc;
            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            mine.transfer(Cin, SubLevel, C, L);
            computeRate(ddC);

            if( stepWasCut(Cex,Cin,ddC,0))
            {
            }

            Solver &F = *this;
            std::cerr << "Ain = " << real_t(F(0)) << std::endl;
            std::cerr << "Aex = " << real_t(F(1)) << std::endl;

            saveProfile("odestep.dat");



        }


    }
}
