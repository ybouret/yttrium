#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/boolean.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Solver:: saveProfile(const String &fn)
        {

            Solver    &F = *this;
            OutputFile fp(fn);
            const size_t np = 1000;
            for(size_t i=0;i<=np;++i)
            {
                const real_t u = double(i)/np;
                fp("%.15g %.15g\n", u, real_t(F(u)));
            }
        }

        void Solver:: nrStage(XWritable &C, const Level L,  XMLog &xml)
        {
            const size_t n = basis.size; assert(n>=2);
            const size_t m = nspc;

            Y_XML_SECTION_OPT(xml, "nrStage", " n='" << n << "' m='" << m << "'");


            {
                int idx = 1;
                for(const PNode *pn=basis.head;pn;pn=pn->next,++idx)
                {
                    const Prospect &pro = (**pn);
                    mine.transfer(Cin,SubLevel,C,L);
                    mine.transfer(Cex,SubLevel,pro.cc, SubLevel);
                    //const String fn = pro.eq.fileName() + ".pro";
                    const String fn = Formatted::Get("pro%d.dat",idx);
                    saveProfile(fn);
                }

            }

            //------------------------------------------------------------------
            //
            //
            // initialize Phi and Nu
            //
            //
            //------------------------------------------------------------------
            XMatrix Phi(n,m);
            XMatrix Nu(n,m);
            XArray  xi(n);

            {
                const PNode *pn = basis.head;
                for(size_t i=1;i<=n;++i,pn=pn->next)
                {
                    assert(0!=pn);
                    const Prospect    &pro = **pn;
                    const Equilibrium &eq  = pro.eq;
                    XWritable         &phi = Phi[i];
                    XWritable         &nu  = Nu[i];
                    eq.topology(nu, SubLevel);
#if 0
                    eq.drvsMassAction(pro.ek, phi, SubLevel, C, L, afm.xmul);
                    (xi[i] = eq.massAction(pro.ek, afm.xmul, C, L)).neg();
#else
                    eq.drvsAffinity(phi, SubLevel, C, L);
                    (xi[i] = eq.affinity(pro.ek, afm.xmul, C, L)).neg();
#endif
                }
            }

            //------------------------------------------------------------------
            //
            //
            // compute Phi*Nu'
            //
            //
            //------------------------------------------------------------------
            XMatrix Chi(n,n);
            XAdd   &xadd = afm.xadd;
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    xadd.free();
                    for(size_t k=m;k>0;--k)
                    {
                        xadd << Phi[i][k] * Nu[j][k];
                    }
                    Chi[i][j] = xadd.sum();
                }
            }
            Y_XMLOG(xml, "Phi=" << Phi);
            Y_XMLOG(xml, "Nu="  << Nu);
            Y_XMLOG(xml, "Chi=" << Chi);
            Y_XMLOG(xml, "lhs=" << xi);

            //------------------------------------------------------------------
            //
            //
            // compute inv(Phi*Nu')
            //
            //
            //------------------------------------------------------------------
            if(!xlu.build(Chi))
            {
                std::cerr << "Singular Matrix" << std::endl;
                return;
            }

            //------------------------------------------------------------------
            //
            //
            // deduce xi and dC
            //
            //
            //------------------------------------------------------------------
            xlu.solve(Chi,xi);
            Y_XMLOG(xml, "xi =" << xi);
            mine.transfer(Cin, SubLevel, C, L);

            for(size_t j=m;j>0;--j)
            {
                xadd.free();
                for(size_t k=n;k>0;--k)
                {
                    xadd << Nu[k][j] * xi[k];
                }
                ddC[j] = xadd.sum();
            }

            //------------------------------------------------------------------
            //
            //
            // scaling
            //
            //
            //------------------------------------------------------------------
            xreal_t    scale;
            const bool abate = stepWasCut(Cex,Cin,ddC,&scale);
            Y_XMLOG(xml, "abate = " << BooleanTo::text(abate) );
            Y_XMLOG(xml, "scale = " << real_t(scale) );

            Solver &F = *this;
            const xreal_t Ain   = objGrad(Cin,SubLevel);
            const xreal_t slope = afm.xadd.dot(ddC,grd);

            std::cerr << "Ain = " << real_t(Ain) << " / " << real_t(objFunc(Cin, SubLevel)) << " / " << real_t(F(0)) << std::endl;
            std::cerr << "Aex = " << real_t(objFunc(Cex, SubLevel)) << " / " << real_t(F(1)) << std::endl;

            std::cerr << "slope=" << real_t(slope) << std::endl;

            {
                OutputFile fp("nrstage.dat");
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


