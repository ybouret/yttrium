#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Solver:: basisToRate(XWritable &rate)
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


        bool MustCut(xreal_t         &scale,
                     const XReadable &C,
                     const XReadable &dC)
        {
            scale = 1.0;
            bool abate = false;

            for(size_t j=C.size();j>0;--j)
            {
                const xreal_t d = dC[j];  if(d.mantissa>=0) continue;
                const xreal_t c = C[j];   assert(c.mantissa>=0);
                const xreal_t f = c/(-d);
                if(f<=scale)
                {
                    abate = true;
                    scale = f;
                }
            }

            return abate;
        }

        void Solver:: odeStep(XWritable &C, const Level L, XMLog &xml)
        {
            const size_t n = basis.size;
            const size_t m = nspc;

            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            // dC = Nu' * xi
            xreal_t scale = 0;
            xreal_t tau   = 1;
            mine.transfer(Cin, SubLevel, C, L);
            basisToRate(ddC);
            Y_XMLOG(xml, "ddC=" << ddC );
            if(MustCut(scale, Cin, ddC))
            {
                tau *= scale * 0.9;
            }

            for(size_t j=m;j>0;--j)
            {
                Cex[j] = Cin[j] + tau * ddC[j];
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
