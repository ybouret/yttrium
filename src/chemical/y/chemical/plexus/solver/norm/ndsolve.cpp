
#include "y/chemical/plexus/solver/normalizer.hpp"
#include "y/text/boolean.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"
#include "y/sort/heap.hpp"

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


            //------------------------------------------------------------------
            //
            //
            // compile from top-level until no replacement
            //
            //
            //------------------------------------------------------------------
        COMPILE:
            while( true )
            {
                bool         repl = false;
                const size_t napp = compile(Ctop, Ktop, repl, xml);
                if(napp<=0) { Y_XMLOG(xml, "[Jammed!]"); return true; }
                if(repl) continue;
                break;
            }


            
            //------------------------------------------------------------------
            //
            //
            // extract most promising basis and check dimension
            //
            //
            //------------------------------------------------------------------
            const size_t n = extract(xml);
            switch(n)
            {
                case 0:
                    return true; // shouldn't happen, jammed

                case 1:
                    assert(0!=apl.head);
                    rcl.transfer(Ctop,TopLevel,(**apl.head).cc,SubLevel);
                    return true;

                default:
                    break;
            }

            if(xml.verbose)
            {
                Y_XML_SECTION(xml, "Basis");
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Applicant &app = **an;
                    app.display(xml() << "|", rcl.uuid, false) << std::endl;
                }
            }


            //------------------------------------------------------------------
            //
            //
            // setup metrics
            //
            //
            //------------------------------------------------------------------
            XAdd    &     xadd = afm.xadd;
            XMul    &     xmul = afm.xmul;
            XMatrix &     phi  = Phi[n];
            XMatrix &     Nu   = XNu[n];
            XMatrix &     chi  = Chi[n];
            XArray  &     xi   = Lhs[n];
            const size_t  m    = nsp;

            //------------------------------------------------------------------
            //
            //
            // fill jacobian in phi and mass action in xi
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
                    eq.drvsMassAction(eK, phi[ii], SubLevel, Ctop, TopLevel, xmul);
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
            xadd.make(m);
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

            //------------------------------------------------------------------
            //
            //
            // compute inv(phi*Nu')
            //
            //
            //------------------------------------------------------------------
            if(!xlu.build(chi))
            {
                Y_XMLOG(xml, "[Singular] => try to fortify");
                if(!fortify(Ctop,Ktop,xml))
                {
                    Y_XMLOG(xml, "[Singular] spurious!!!");
                    return false;
                }
                goto COMPILE;
            }

            //------------------------------------------------------------------
            //
            //
            // compute numeric extent and starting position
            //
            //
            //------------------------------------------------------------------
            xlu.solve(chi,xi);
            rcl.transfer(Cin, SubLevel, Ctop, TopLevel);

            //------------------------------------------------------------------
            //
            //
            // compute dC and deduce scaling
            //
            //
            //------------------------------------------------------------------
            const   xreal_t zero  = 0;
            const   xreal_t one   = 1;
            const   xreal_t smax  = 2;
            xreal_t         scale = one;
            bool            abate = false;

            for(size_t j=m;j>0;--j)
            {
                assert(xadd.isEmpty());
                for(size_t k=n;k>0;--k)
                {
                    const xreal_t p = Nu[k][j] * xi[k];
                    xadd << p;
                }
                const xreal_t dC = Cws[j] = xadd.sum();
                if(dC<zero)
                {
                    const xreal_t factor = Cin[j]/(-dC);
                    if(!abate)
                    {
                        abate = true;
                        scale = factor;
                    }
                    else
                    {
                        if(factor<scale) scale = factor;
                    }
                }
            }


            Y_XMLOG(xml,"scale = " << std::setw(15) << real_t(scale));
            Y_XMLOG(xml,"abate = " << std::setw(15) << BooleanTo::Text(abate) );

            if(abate)
            {
                // decrease scale
                scale *= 0.99;

                // doesn't go that far if scale is big
                if(scale>smax)
                    scale=smax;
            }
            else
            {
                // unlikely, no negative dC
                scale = smax;
            }



            //------------------------------------------------------------------
            //
            //
            // compute Cex as the (truncated or expanded) Newton's step
            //
            //
            //------------------------------------------------------------------
            for(size_t j=m;j>0;--j)
                Cex[j] = Cin[j] + scale * Cws[j];


            Y_XMLOG(xml,"scale = " << std::setw(15) << real_t(scale));


            //------------------------------------------------------------------
            //
            //
            // prepare minimization step
            //
            //
            //------------------------------------------------------------------
            Triplet<xreal_t> xx = {         zero,    -1,          one  };
            Triplet<xreal_t> ff = { (*this)(xx.a), xx.b, (*this)(xx.c) };
            const xreal_t    f0 = ff.a;

            if(true)
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
            const xreal_t score = (*this)(u_opt);
            xreal_t       limit = 0;
            {
                const ANode *an = apl.head;
                limit = (**an).ff;
                for(an=an->next;an;an=an->next)
                    limit = Min(limit,(**an).ff);
            }

            Y_XMLOG(xml, "score = " << std::setw(15) << real_t(score) << " (<= " << real_t(f0) << ")");
            Y_XMLOG(xml, "limit = " << std::setw(15) << real_t(limit));



            if(score<limit)
            {
                // take the step
                rcl.transfer(Ctop, TopLevel, Cws, SubLevel);
                Y_XMLOG(xml, "[Success]");
                return true;
            }
            else
            {
                // fallback to simplex
                Y_XMLOG(xml, "[Partial]");
                return  improve(Ctop, false, xml);
            }

        }


    }

}


