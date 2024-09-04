
#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"

#include "y/chemical/plexus/solver.hpp"


#include "y/sort/heap.hpp"

#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

#include "y/utest/run.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        
      

        void Solver:: nrStage(XWritable &C, const Level L,  XMLog &xml)
        {
            const size_t n = basis.size;
            const size_t m = nspc;

            Y_XML_SECTION_OPT(xml, "nrStage", " n='" << n << "' m='" << m << "'");

            //------------------------------------------------------------------
            //
            //
            //
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
                    eq.drvsMassAction(pro.ek, phi, SubLevel, C, L, afm.xmul);
                    eq.topology(nu, SubLevel);
                    (xi[i] = eq.massAction(pro.ek, afm.xmul, C, L)).neg();
                }
            }


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
            //Y_XMLOG(xml, "Phi=" << Phi);
            Y_XMLOG(xml, "Nu="  << Nu);
            Y_XMLOG(xml, "Chi=" << Chi);
            Y_XMLOG(xml, "lhs=" << xi);

            if(!xlu.build(Chi))
            {
                std::cerr << "Singular Matrix" << std::endl;
                return;
            }

            xlu.solve(Chi,xi);
            Y_XMLOG(xml, "xi =" << xi);
            mine.transfer(Cin, SubLevel, C, L);
            bool    abate = false;
            xreal_t scale = 1.0;

            for(size_t j=m;j>0;--j)
            {
                xadd.free();
                for(size_t k=n;k>0;--k)
                {
                    xadd << Nu[k][j] * xi[k];
                }
                const xreal_t d = (ddC[j] = xadd.sum());
                if(d.mantissa<0)
                {
                    const xreal_t c = Cin[j];
                    const xreal_t f = c/(-d);
                    if(f<=scale)
                    {
                        abate = true;
                        scale = f;
                    }
                }
            }

            Y_XMLOG(xml, "abate = " << abate);
            Y_XMLOG(xml, "scale = " << real_t(scale) );

            if(xml.verbose)
            {
                for(size_t j=1;j<=m;++j)
                {
                    xml() << "C = " << std::setw(15) << real_t(Cin[j]) << " | dC = " << std::setw(15) << real_t(ddC[j]) << std::endl;
                }
            }

            if(abate)
            {
                scale *= 0.99;
                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + scale * ddC[j];
                }
            }
            else
            {
                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + ddC[j];
                }
            }

            Solver &F = *this;
            std::cerr << "Ain = " << real_t(objFunc(Cin, SubLevel)) << " / " << real_t(F(0)) << std::endl;
            std::cerr << "Aex = " << real_t(objFunc(Cex, SubLevel)) << " / " << real_t(F(1)) << std::endl;

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

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(solver)
{

    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    Clusters        &cls = plexus.assemble();
    const XReadable &K   = cls.K(0);
    Wardens          ward(cls);

    const Library    &lib = plexus.lib;
    //const Equilibria &eqs = plexus.eqs;
    XMLog            &xml = plexus.xml;

    XVector C0(lib->size(),0);
    XVector dC(C0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Solver solver(*cl);

        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.5);
            lib(std::cerr << "C0=","\t[",C0,"]");
            ward(C0,dC,TopLevel,xml);
            lib(std::cerr << "C0=","\t[",C0,"]");
            solver.process(C0, TopLevel, K, xml);

            lib(std::cerr << "C1=","\t[",C0,"]");

        }
    }




}
Y_UDONE()
