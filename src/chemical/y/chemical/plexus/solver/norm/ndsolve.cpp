
#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        bool Normalizer:: NDSolve(XWritable       &Ctop,
                                  const XReadable &Ktop,
                                  XMLog           &xml)
        {
            Y_XML_SECTION(xml, "NDSolve");

            bool          repl = false;
            const size_t  nmax = compile(Ctop, Ktop, repl, xml);
            if( nmax <=0) { Y_XMLOG(xml, "[Jammed!]"); return true; }

            if(xml.verbose)
            {
                for(size_t i=1;i<=nmax;++i)
                {
                    const Equilibrium &eq = aps[i].eq;
                    rcl.display(std::cerr, eq, Ktop) << std::endl;
                }
            }

            const size_t  n = extract(xml);
            const size_t  m = nsp;

            // act on n!

            XMatrix &Nu  = XNu[n];
            XMatrix &phi = Phi[n];
            XMatrix &chi = Chi[n];
            XArray  &lhs = Lhs[n];

            // fill jacobian and lhs
            {
                size_t ii=0;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    ++ii;
                    const Applicant   &app = **an;
                    const Equilibrium &eq  =  app.eq;
                    const xreal_t      eK  =  app.eK;
                    lhs[ii] = -eq.massAction(eK, afm.xmul, Ctop, TopLevel);
                    eq.topology(Nu[ii], SubLevel);
                    eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, afm.xmul);
                }
            }


            // compute phi*Nu'
            XAdd &xadd = afm.xadd;
            xadd.make(n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    assert(xadd.isEmpty());
                    for(size_t k=m;k>0;--k)
                    {
                        const xreal_t p = phi[i][k] * Nu[j][k];
                        xadd << p;
                    }
                    chi[i][j] = xadd.sum();
                }
            }

            MKL::LU<xreal_t> lu(n);


            std::cerr << "phi = " << phi << std::endl;
            std::cerr << "Nu  = " << Nu << std::endl;
            std::cerr << "lhs = " << lhs << std::endl;
            std::cerr << "chi = " << chi << std::endl;

            if(!lu.build(chi))
            {
                Y_XMLOG(xml, "singular");
                return false;
            }


            // compute extent in lhs
            lu.solve(chi,lhs);
            std::cerr << "xi=" << lhs << std::endl;
            rcl.transfer(Cin, SubLevel, Ctop, TopLevel);

            assert(xadd.isEmpty());
            bool            cut = false;
            xreal_t         fac = 1;
            const   xreal_t zero;

            for(size_t j=m;j>0;--j)
            {
                for(size_t k=n;k>0;--k)
                {
                    const xreal_t p = Nu[k][j] * lhs[k];
                    xadd << p;
                }
                const xreal_t dC = Cws[j] = xadd.sum();
                if(dC<zero)
                {
                    const xreal_t scale = Cin[j]/(-dC);
                    if(scale<fac)
                    {
                        cut = true;
                        fac = scale;
                    }
                }
            }

            std::cerr << "C0  = " << Cin << std::endl;
            std::cerr << "dC  = " << Cws << std::endl;
            std::cerr << "cut = " << cut << std::endl;
            std::cerr << "fac = " << fac << std::endl;


            if(cut) {
                fac /= 2;
                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + fac * Cws[j];
                }
            }
            else
            {
                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + Cws[j];
                }
            }


            std::cerr << "C1  = " << Cex << std::endl;
            for(const ANode *an=apl.head;an;an=an->next)
            {
                const Equilibrium &eq = (**an).eq;
                if(eq.reac.deficient(Cex, SubLevel)) throw Specific::Exception(eq.name.c_str(),"deficient in reactant(s)");
                if(eq.prod.deficient(Cex, SubLevel)) throw Specific::Exception(eq.name.c_str(),"deficient in product(s)");
            }

            Triplet<xreal_t> xx = { 0, -1, 1 };
            Triplet<xreal_t> ff = { (*this)(xx.a), -1, (*this)(xx.c) };
            const xreal_t    f0 = ff.a;

            std::cerr << "xx=" << xx << ", ff=" << ff << std::endl;


            {
                OutputFile fp("ndsolve.dat");
                const size_t np(1000);
                for(size_t j=0;j<=np;++j)
                {
                    const real_t u = real_t(j)/np;
                    fp("%.15g %.15g\n", u,  (real_t( (*this)(u) ) ) );
                }
                fp << "\n";
            }

            const xreal_t u_opt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
            const xreal_t cost  = (*this)(u_opt);
            Y_XMLOG(xml, "affinity= " << real_t(f0) << " -> " << real_t(cost) );
            rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

            return f0 < cost;
        }

    }

}

