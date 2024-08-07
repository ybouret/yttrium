
#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

#include "y/mkl/tao/seq/level1.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;

        Normalizer::Result Normalizer:: NDSolve(XWritable       &Ctop,
                                                const XReadable &Ktop,
                                                XMLog           &xml)
        {
            Y_XML_SECTION(xml, "NDSolve");

            //------------------------------------------------------------------
            //
            //
            // compile from top-level until no replacement
            //
            //
            //------------------------------------------------------------------
            while( true )
            {
                bool         repl = false;
                const size_t napp = compile(Ctop, Ktop, repl, xml);
                if( napp <=0) { Y_XMLOG(xml, "[Jammed!]"); return Success; }
                if(repl) continue;
                break;
            }



            //------------------------------------------------------------------
            //
            //
            // extract basis and check dimension
            //
            //
            //------------------------------------------------------------------
            const size_t  n = extract(xml);
            switch(n)
            {
                case 0:
                    return Success; // shouldn't happen, jammed

                case 1:
                    rcl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel);
                    return Success;

                default:
                    break;
            }


            //------------------------------------------------------------------
            //
            //
            //
            // get metrics
            //
            //
            //------------------------------------------------------------------
            XMatrix &     Nu  = XNu[n];
            XMatrix &     phi = Phi[n];
            XMatrix &     chi = Chi[n];
            XArray  &     xi  = Lhs[n];
            const size_t  m   = nsp;

            //------------------------------------------------------------------
            //
            //
            // fill jacobian and lhs
            //
            //
            //------------------------------------------------------------------
            {
                size_t ii=0;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    ++ii;
                    const Applicant   &app = **an;
                    const Equilibrium &eq  =  app.eq;
                    const xreal_t      eK  =  app.eK;
                    const xreal_t      ma  = eq.massAction(eK, afm.xmul, Ctop, TopLevel);
                    xi[ii] = -ma;
                    eq.topology(Nu[ii], SubLevel);
                    eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, afm.xmul);
                    Y_XMLOG(xml, "ma = " << std::setw(15) << real_t(ma) << " | xi = " << std::setw(15) << real_t(app.xi) << " @" << eq.name);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // compute phi*Nu'
            //
            //
            //------------------------------------------------------------------
            XAdd &xadd = afm.xadd;
            xadd.make(n);
            for(size_t i=n;i>0;--i)
            {
                const XReadable &phi_i=phi[i];
                for(size_t j=n;j>0;--j)
                {
                    const XReadable &Nu_j = Nu[j];
                    assert(xadd.isEmpty());
                    for(size_t k=m;k>0;--k)
                    {
                        const xreal_t p = phi_i[k] * Nu_j[k];
                        xadd << p;
                    }
                    chi[i][j] = xadd.sum();
                }
            }



            std::cerr << "phi = " << phi << std::endl;
            std::cerr << "Nu  = " << Nu  << std::endl;
            std::cerr << "lhs = " << xi  << std::endl;
            std::cerr << "chi = " << chi << std::endl;

            //------------------------------------------------------------------
            //
            //
            // compute inv(phi*Nu')
            //
            //
            //------------------------------------------------------------------
            if(!xlu.build(chi))
            {
                Y_XMLOG(xml, "singular");
                return Failure;
            }

            //------------------------------------------------------------------
            //
            //
            // compute extent
            //
            //
            //------------------------------------------------------------------
            xlu.solve(chi,xi);
            std::cerr << "xi  = " << xi << std::endl;

            //------------------------------------------------------------------
            //
            //
            // compute dC and scaling
            //
            //
            //------------------------------------------------------------------
            rcl.transfer(Cin, SubLevel, Ctop, TopLevel);
            const   xreal_t zero  = 0;
            const   xreal_t one   = 1;
            xreal_t         scale = one;
            bool            first = true;

            for(size_t j=m;j>0;--j)
            {
                for(size_t k=n;k>0;--k)
                {
                    const xreal_t p = Nu[k][j] * xi[k];
                    xadd << p;
                }
                const xreal_t dC = Cws[j] = xadd.sum();
                if(dC<zero)
                {
                    const xreal_t factor = Cin[j]/(-dC);
                    std::cerr << "factor=" << real_t(factor) << std::endl;
                    if(first)
                    {
                        first = false;
                        scale = factor;
                    }
                    else
                    {
                        if(factor<scale) scale = factor;
                    }
                }
            }

            const bool abate = scale < one;

            std::cerr << "C0    = " << Cin << std::endl;
            std::cerr << "dC    = " << Cws << std::endl;
            std::cerr << "scale = " << real_t(scale) << std::endl;
            std::cerr << "abate = " << abate         << std::endl;

            if( abate )
            {
                // scale /= 2
                --Coerce(scale.exponent);
            }
            else
            {
                
            }



            return Failure;


#if 0
            //------------------------------------------------------------------
            //
            //
            // compile status from top-level and extract basis
            //
            //
            //------------------------------------------------------------------
            bool          repl = false;
            const size_t  nmax = compile(Ctop, Ktop, repl, xml);
            if( nmax <=0) { Y_XMLOG(xml, "[Jammed!]"); return Success; }
            const size_t  n = extract(xml);

            //------------------------------------------------------------------
            //
            //
            // switch on base size
            //
            //
            //------------------------------------------------------------------
            switch(n)
            {
                case 0:
                    return Success; // shouldn't happen, jammed

                case 1:
                    rcl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel);
                    return Success;

                default:
                    break;
            }


            //------------------------------------------------------------------
            //
            //
            //
            // get metrics
            //
            //
            //------------------------------------------------------------------
            XMatrix &     Nu  = XNu[n];
            XMatrix &     phi = Phi[n];
            XMatrix &     chi = Chi[n];
            XArray  &     xi  = Lhs[n];
            const size_t  m   = nsp;

            //------------------------------------------------------------------
            //
            //
            // fill jacobian and lhs
            //
            //
            //------------------------------------------------------------------
            {
                size_t ii=0;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    ++ii;
                    const Applicant   &app = **an;
                    const Equilibrium &eq  =  app.eq;
                    const xreal_t      eK  =  app.eK;
                    const xreal_t      ma  = eq.massAction(eK, afm.xmul, Ctop, TopLevel);
                    xi[ii] = -ma;
                    eq.topology(Nu[ii], SubLevel);
                    eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, afm.xmul);
                    Y_XMLOG(xml, "ma = " << std::setw(15) << real_t(ma) << " | xi = " << std::setw(15) << real_t(app.xi) << " @" << eq.name);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // compute phi*Nu'
            //
            //
            //------------------------------------------------------------------
            XAdd &xadd = afm.xadd;
            xadd.make(n);
            for(size_t i=n;i>0;--i)
            {
                const XReadable &phi_i=phi[i];
                for(size_t j=n;j>0;--j)
                {
                    const XReadable &Nu_j = Nu[j];
                    assert(xadd.isEmpty());
                    for(size_t k=m;k>0;--k)
                    {
                        const xreal_t p = phi_i[k] * Nu_j[k];
                        xadd << p;
                    }
                    chi[i][j] = xadd.sum();
                }
            }

            MKL::LU<xreal_t> lu(n);


            std::cerr << "phi = " << phi << std::endl;
            std::cerr << "Nu  = " << Nu  << std::endl;
            std::cerr << "lhs = " << xi  << std::endl;
            std::cerr << "chi = " << chi << std::endl;

            //------------------------------------------------------------------
            //
            //
            // compute inv(phi*Nu')
            //
            //
            //------------------------------------------------------------------
            if(!lu.build(chi))
            {
                Y_XMLOG(xml, "singular");
                return Failure;
            }

            //------------------------------------------------------------------
            //
            //
            // compute extent in lhs
            //
            //
            //------------------------------------------------------------------
            lu.solve(chi,xi);
            std::cerr << "xi  = " << xi << std::endl;
            rcl.transfer(Cin, SubLevel, Ctop, TopLevel);

            assert(xadd.isEmpty());
            bool            cut = false;
            xreal_t         fac = 1;
            const   xreal_t zero;

            for(size_t j=m;j>0;--j)
            {
                for(size_t k=n;k>0;--k)
                {
                    const xreal_t p = Nu[k][j] * xi[k];
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
            std::cerr << "fac = " << real_t(fac) << std::endl;


            if(cut) {
                // fac /= 2;
                --Coerce(fac.exponent);
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

            const xreal_t uopt = Minimize<xreal_t>::Locate(Minimizing::Inside, *this, xx, ff);
            const xreal_t cost  = (*this)(uopt);
            Y_XMLOG(xml, "affinity= " << real_t(f0) << " -> " << real_t(cost) );
            rcl.transfer(Ctop, TopLevel, Cws, SubLevel);

            if(cost < f0)
            {
                return cut ? Trimmed : Success;
            }
            else
            {
                return Failure;
            }
#endif
        }


#if 0
        void Normalizer::LookUp(XWritable &Ctop, const XReadable &Ktop, XMLog &xml)
        {

            Y_XML_SECTION(xml, "LookUp");

            bool          repl = false;
            const size_t  nmax = compile(Ctop, Ktop, repl, xml);
            if( nmax <=0) { Y_XMLOG(xml, "[Jammed!]"); return; }


            const size_t  n = extract(xml);
            const size_t  m = nsp;

            // act on n!

            XMatrix &Nu  = XNu[n];
            XMatrix &phi = Phi[n];
            XMatrix &ups = Chi[n];
            XArray  &lhs = Lhs[n];

            // Nu, phi and lhs=xi
            {
                size_t ii=0;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    ++ii;
                    const Applicant   &app = **an;
                    const Equilibrium &eq  =  app.eq;
                    const xreal_t      eK  =  app.eK;
                    eq.topology(Nu[ii], SubLevel);
                    eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, afm.xmul);
                    lhs[ii] = app.xi;

                    Y_XMLOG(xml, "xi = " << std::setw(15) << real_t(app.xi) << " @" << eq.name);
                }
            }

            std::cerr << "lhs=" << lhs << std::endl;
            XAdd &xadd = afm.xadd;
            for(size_t i=1;i<=n;++i)
            {
                xadd.make(m);
                const xreal_t den = Tao::DotProduct<xreal_t>::Of_(phi[i], Nu[i], xadd);
                //std::cerr << "den=" << den << std::endl;
                ups[i][i] = 1;
                for(size_t j=1;j<i;++j)    ups[i][j] = Tao::DotProduct<xreal_t>::Of_(phi[i], Nu[j], xadd)/den;
                for(size_t j=i+1;j<=n;++j) ups[i][j] = Tao::DotProduct<xreal_t>::Of_(phi[i], Nu[j], xadd)/den;

            }
            std::cerr << "ups=" << ups << std::endl;
            CxxArray<xreal_t> xi(n,0);
            for(size_t i=1;i<=n;++i)
            {
                xadd.make(n);
                for(size_t j=1;j<=n;++j)
                {
                    xadd << ups[j][i] * lhs[j];
                }
                xi[i] = xadd.sum();
            }
            std::cerr << "xi=" << xi << std::endl;
            std::cerr << "Nu=" << Nu << std::endl;
            for(size_t j=1;j<=m;++j)
            {
                xadd.make(n);
                for(size_t i=1;i<=n;++i)
                {
                    xadd << Nu[i][j] * xi[i];
                }
                Cws[j] = xadd.sum();
            }
            std::cerr << "dC=" << Cws << std::endl;
        }

#endif
        
    }
}

