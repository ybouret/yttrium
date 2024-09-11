#include "y/chemical/plexus/solver.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/boolean.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

      

        bool Solver:: nraStep(XMLog &xml)
        {
            const size_t n = basis.size; assert(n>=2);
            const size_t m = nspc;

            // incoming with basis, ff0, grd, and Cin
            Y_XML_SECTION_OPT(xml, NRA_Step, " n='" << n << "' m='" << m << "'");



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
                    eq.drvsAffinity(phi, SubLevel, Cin, SubLevel);
                    (xi[i] = eq.affinity(pro.ek, afm.xmul, Cin, SubLevel)).neg();
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
                return false;
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
            // Compute Cex by acceptable ddC
            //
            //
            //------------------------------------------------------------------
            {
                xreal_t    scale;
                if( stepWasCut(Cex,Cin,ddC,&scale) )
                {
                    Y_XMLOG(xml, "# scale = " << real_t(scale) );
                }
                else
                {
                    Y_XMLOG(xml, "# full step");
                }
            }

            saveProfile(NRA_Step,1000);

            //------------------------------------------------------------------
            //
            //
            // Compute slope to see if acceptable direction
            //
            //
            //------------------------------------------------------------------
            const xreal_t slope = afm.xadd.dot(ddC,grd);
            Y_XMLOG(xml,"slope = " << real_t(slope));
            if(slope.mantissa>=0.0)
            {
                Y_XMLOG(xml, "positive slope");
                return false;
            }

            Triplet<xreal_t> uu = { 0,   -1, 1 };
            Triplet<xreal_t> ff = { ff0, -1, objFunc(Cex,SubLevel) };
            const xreal_t    uu1 = Minimize<xreal_t>::Locate(Minimizing::Inside, fcn, uu, ff);
            const xreal_t    ff1 = fcn(uu1);

            Y_XMLOG(xml, "ff0   = " << real_t(ff0));
            Y_XMLOG(xml, "ff1   = " << real_t(ff1) <<  " @" << real_t(uu1));


            if(ff1<=ff0)
            {
                vpush(Cws,ff1);
                return true;
            }
            else return false;
        }

    }

}


