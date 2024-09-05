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

#if 0
            // collect of increases from current basis
            for(const PNode *pn=basis.head;pn;pn=pn->next)
                (**pn).step(inc);
#else
            for(size_t j=pps.size();j>0;--j)
            {
                pps[j].step(inc);
            }
#endif
            // deduce rate
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = inc[j].sum();
            }

        }



        void Solver:: odeStep(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
        {
            const size_t n = basis.size;
            const size_t m = nspc;
            Y_XML_SECTION_OPT(xml, "odeStep", " n='" << n << "' m='" << m << "'");

            // assume starting from an upgraded state
            XArray C0(m);
            XArray r1(m);
            XArray r2(m);
            XArray k1(m);
            XArray k2(m);
            XArray C1(m);
            XArray C2(m);


            // dC = Nu' * xi
            xreal_t full   = 1;
            xreal_t half   = 0.5;
            mine.transfer(C0, SubLevel, C, L);
            computeRate(r1);

        EvalK1:
            for(size_t j=m;j>0;--j)
            {
                k1[j] = r1[j] * half;
            }


            {
                xreal_t scale;
                if(stepWasCut(C1, C0, k1, &scale))
                {
                    Y_XMLOG(xml, "cut k1 by " << real_t(scale));
                    full *= scale;
                    half *= scale;
                }
            }

            Y_XMLOG(xml, "C0=" << C0 );
            Y_XMLOG(xml, "C1=" << C1 );

            // C1 = C0 + half * r0
            upgrade(C1, L, Ktop, xml);
            computeRate(r2);
            for(size_t j=m;j>0;--j)
            {
                k2[j] = full * r2[j];
            }

            {
                xreal_t scale;
                if(stepWasCut(C2, C0, k2, &scale))
                {
                    Y_XMLOG(xml, "cut k2 by " << real_t(scale));
                    full *= scale;
                    half *= scale;
                    goto EvalK1;
                }
            }

            Y_XMLOG(xml, "C0=" << C0 );
            Y_XMLOG(xml, "C1=" << C1 );
            Y_XMLOG(xml, "C2=" << C2 );
            Y_XMLOG(xml, "dt=" << full );

            for(size_t j=m;j>0;--j)
            {
                Cin[j] = C0[j];
                Cex[j] = C2[j];
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
