

#include "y/chemical/plexus.hpp"
#include "y/chemical/reactive/aftermath.hpp"

#include "y/utest/run.hpp"

#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"
#include "y/text/boolean.hpp"

#include "y/stream/libc/output.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/algebra/lu.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        using namespace MKL;

        class Prospect
        {
        public:
            typedef CxxSeries<Prospect,XMemory> Series;

            Prospect(const Equilibrium &_eq,
                     const xreal_t      _eK,
                     const XReadable   &_cc,
                     const xreal_t      _xi,
                     const XReadable   &_dc) :
            eq(_eq),
            eK(_eK),
            cc(_cc),
            xi(_xi),
            ax(xi.abs()),
            dc(_dc)
            {
            }

            Prospect(const Prospect &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax),
            dc(_.dc)
            {
            }

            ~Prospect() noexcept {}

            std::ostream & show(std::ostream &os, const Cluster &cl, const bool full) const
            {
                cl.uuid.pad(os << eq.name, eq) << "|xi=" << std::setw(15) << real_t(xi);
                if(full)
                {
                    eq.displayCompact(os << " @", cc, SubLevel);
                }
                return os;
            }

            xreal_t objectiveFunction(XMul            &X,
                                      const XReadable &C,
                                      const Level      L) const
            {
                return eq.affinity(eK,X,C,L);
            }

            static int CompareAX(const Prospect &lhs, const Prospect &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.ax, rhs.ax);
            }

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ax;
            const XReadable   &dc;

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        using namespace MKL;

        class Cracker : public Quantized, public Counted
        {
        public:

            explicit Cracker(const Cluster &cl) :
            rcl(cl),
            afm(),
            neq(cl.size),
            nsp(cl.species.size),
            ceq(neq,nsp),
            ddc(neq,nsp),
            Phi(neq,nsp),
            Jac(nsp,nsp),
            Den(nsp,nsp),
            Jdg(nsp),
            Jxv(nsp),
            Cin(nsp),
            Cws(nsp),
            Cex(nsp),
            rho(nsp),
            pps(neq),
            obj(neq),
            xdc(nsp,CopyOf,neq),
            xlu(nsp)
            {
            }


            virtual ~Cracker() noexcept
            {
            }

            //! remove all crucial
            void slacken(XWritable       & C,
                         const Level       L,
                         const XReadable & Ktop,
                         XMLog &           xml)
            {
                Y_XML_SECTION(xml, "Slacken");

                //--------------------------------------------------------------
                //
                //
                // collect series of crucial equilibri(um|a)
                //
                //
                //--------------------------------------------------------------
                size_t cycle = 0;
            DETECT_CRUCIAL:
                ++cycle;
                pps.free();
                for(const ENode *en = rcl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en; if( !eq.crucial(C,L) ) continue;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = pps.size()+1;
                    XWritable         &cc = rcl.transfer(ceq[ii],SubLevel,C,L);
                    XWritable         &dc = ddc[ii];
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);
                    if(Crucial!=st) throw Specific::Exception(eq.name.c_str(), "corrupted crucial state");

                    pps << Prospect(eq, eK, cc, afm.eval(dc, cc, SubLevel, C, L, eq), dc);
                }

                //--------------------------------------------------------------
                //
                //
                // move the smallest correction if any
                //
                //
                //--------------------------------------------------------------
                if( pps.size() )
                {
                    Y_XMLOG(xml, "(@) cycle #" << cycle);
                    HeapSort::Call(pps,Prospect::CompareAX);
                    const size_t n = pps.size();
                    if(xml.verbose)
                        for(size_t i=1;i<=n;++i)
                            pps[i].show(xml() << "(!) ", rcl, false) << std::endl;

                    const Prospect &pro = pps[n];
                    rcl.transfer(C, L, pro.cc, SubLevel);
                    goto DETECT_CRUCIAL; // then check again
                }
            }

            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(size_t i=pps.size();i>0;--i)
                {
                    obj << pps[i].objectiveFunction(afm.xmul, C, L);
                }
                return afm.xadd.normOf(obj);
            }


            //! compile running prospect(s), none must be crucial
            size_t compile(const XReadable & C,
                           const Level       L,
                           const XReadable & Ktop,
                           XMLog &           xml)
            {
                Y_XML_SECTION(xml, "Compile");
                //--------------------------------------------------------------
                //
                //
                // collect all running equilbri(um|a)
                //
                //--------------------------------------------------------------
                pps.free();
                for(const ENode *en = rcl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = pps.size()+1;
                    XWritable         &cc = rcl.transfer(ceq[ii],SubLevel,C,L);
                    XWritable         &dc = ddc[ii];
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked: continue;
                        case Crucial: throw Specific::Exception(eq.name.c_str(), "unexpected crucial state!");
                        case Running:
                            break;
                    }

                    const Prospect pro(eq, eK, cc, afm.eval(dc, cc, SubLevel, C, L, eq), dc);
                    pps << pro;

                }

                HeapSort::Call(pps,Prospect::CompareAX);
                const size_t n = pps.size();
                for(size_t i=1;i<=n;++i)
                {
                    const Prospect &pro = pps[i];
                    if(xml.verbose) pro.show(xml() << "(+) ", rcl, true) << std::endl;
                    mustSupport(pro.cc,SubLevel);
                }

                return n;
            }

            void mustSupport(const XReadable &C, const Level L) const
            {
                for(size_t i=pps.size();i>0;--i)
                {
                    const Prospect &pro = pps[i];
                    pro.eq.mustSupport(C,L);
                }
            }


            void makePhiRaw( )
            {
                Phi.ld(0);
                for(size_t i=pps.size();i>0;--i)
                {
                    const Prospect    &pro = pps[i];
                    const Equilibrium &eq  = pro.eq;
                    const size_t       ii  = eq.indx[SubLevel];
                    XWritable         &phi = Phi[ii];
                    eq.drvsMassAction(pro.eK, phi, SubLevel, pro.cc, SubLevel, afm.xmul);

                }
            }

            // need a pre-compilation
            void makePhiJac()
            {
                Phi.ld(0);
                const size_t m = nsp;
                for(size_t i=pps.size();i>0;--i)
                {
                    const Prospect    &pro = pps[i];
                    const Equilibrium &eq  = pro.eq;
                    const size_t       ii  = eq.indx[SubLevel];
                    XWritable         &phi = Phi[ii];
                    eq.drvsMassAction(pro.eK, phi, SubLevel, pro.cc, SubLevel, afm.xmul);

                    const xreal_t den = -afm.xadd.dot(rcl.topology[ii],phi); assert(den>0.0);
                    for(size_t j=m;j>0;--j) phi[j]/= den;

                }
            }


            //! compute jacobian from pre-compiled state
            void makeJac()
            {
                makePhiJac();
                XAdd &              xadd = afm.xadd;
                const size_t        m    = nsp;
                const size_t        n    = neq;
                const XMatrix     & Nu   = rcl.topology;

                for(size_t i=m;i>0;--i)
                {
                    for(size_t j=m;j>0;--j)
                    {
                        xadd.free();
                        for(size_t k=n;k>0;--k)
                        {
                            const xreal_t lhs = Nu[k][i];
                            const xreal_t rhs = Phi[k][j];
                            const xreal_t tmp = lhs * rhs;
                            xadd << tmp;
                        }
                        Jac[i][j] = xadd.sum();
                    }
                }

                for(size_t i=m;i>0;--i)
                {
                    Jdg[i] = Jac[i][i];
                    xadd.free();
                    for(size_t j=m;j>i;--j)   xadd << Jac[i][j].abs();
                    for(size_t j=i-1;j>0;--j) xadd << Jac[i][j].abs();
                    Jxv[i] = xadd.sum();
                }
            }

            //! compute dC from pre-compiled state
            void makeXdC(XWritable &dC)
            {
                const size_t m = nsp;
                for(size_t j=m;j>0;--j) xdc[j].free();

                for(size_t i=pps.size();i>0;--i)
                {
                    const Prospect  &pro = pps[i];
                    const XReadable &dc  = pro.dc;
                    for(size_t j=m;j>0;--j)
                        xdc[j] << dc[j];
                }

                for(size_t j=m;j>0;--j) dC[j] = xdc[j].sum();
            }

            void compute(XWritable       &dC,
                         const XReadable &C,
                         const XReadable &Ktop,
                         XMLog           &xml)
            {
                const size_t dof = pps.size();
                const size_t n   = compile(C, SubLevel, Ktop, xml);
                if(n!=dof) throw Specific::Exception("here", "bad compilation");
                makeXdC(dC);
                makeJac();
            }

            //! rescale to largest step
            bool rescale(xreal_t         &step,
                         const XReadable &dC,
                         const XReadable &C) const
            {

                const xreal_t zero; assert(step>zero);
                bool          wasCut = false;

                for(size_t j=nsp;j>0;--j)
                {
                    const xreal_t d = dC[j];
                    if(d<zero)
                    {
                        const xreal_t c = C[j];   assert(c>zero);
                        const xreal_t s = c/(-d); // max step
                        if(s<=step)
                        {
                            wasCut = true;
                            step   = s;
                        }
                    }
                }

                return wasCut;
            }

#if 0
            //! rescale for jacobian
            bool rescale(xreal_t &step)
            {
                const xreal_t one = 1;
                bool          cut = false;
            SCAN:
                for(size_t j=nsp;j>0;--j)
                {
                    const xreal_t dg = (one - step * Jdg[j]).abs();
                    const xreal_t xv = step * Jxv[j];
                    if(dg<=xv)
                    {
                        cut = true;
                        --Coerce(step.exponent);
                        goto SCAN;
                    }
                }
                return cut;
            }

            //! build (I-tau*Jac)^(-1)
            void getDen(const xreal_t tau)
            {
                const xreal_t one = 1;
                const size_t  m   = nsp;
                for(size_t i=m;i>0;--i)
                {
                    for(size_t j=m;j>0;--j)
                    {
                        Den[i][j] = - tau * Jac[i][j];
                    }
                    Den[i][i] += one;
                }


                if( !xlu.build(Den) )
                {
                    throw Specific::Exception("here", "singular denominator");
                }
            }
#endif

            void run(XWritable       &C,
                     const Level      L,
                     const XReadable &Ktop,
                     XMLog           &xml)
            {
                //--------------------------------------------------------------
                //
                //
                // first time check
                //
                //
                //--------------------------------------------------------------
                slacken(C, L, Ktop, xml);

                //--------------------------------------------------------------
                //
                //
                // initialize
                //
                //
                //--------------------------------------------------------------
                const size_t n = compile(C,L,Ktop,xml);
                switch(n)
                {
                    case 0: Y_XMLOG(xml, "[inactive]"); return;
                    case 1: Y_XMLOG(xml, "[singulet]");
                    {
                        const Prospect &pro = pps[1];
                        rcl.transfer(C, L, pro.cc, SubLevel);
                    } return;

                    default:
                        break;
                }

                assert(n>=2);
                XAdd &xadd = afm.xadd;
                const xreal_t SAFE = 0.9;
                const real_t  two  = 2.0;
                const xreal_t six  = 6.0;
                const xreal_t half = 0.5;
                const size_t  m = nsp;

                XArray k1(m),k2(m),k3(m),k4(m),C2(m),C3(m),C4(m);
                XArray dC(m);


                rcl.transfer(Cin, SubLevel, C, L);
                makeXdC(k1); // from initial compilation


                {
                    xreal_t fullStep  =  1;
                    xreal_t halfStep  = 0.5;
                    if( rescale(halfStep, k1, Cin) )
                    {
                        halfStep *= SAFE;
                        fullStep  = halfStep*two;
                        Y_XMLOG(xml, "full1=" << fullStep);
                    }

                COMPUTE2:
                    for(size_t j=m;j>0;--j)
                    {
                        C2[j] = Cin[j] + k1[j] * halfStep;
                    }
                    mustSupport(C2,SubLevel);

                    compute(k2, C2, Ktop, xml);

                    if( rescale(halfStep,k2,C2) )
                    {
                        halfStep *= SAFE;
                        fullStep  = halfStep*two;
                        Y_XMLOG(xml, "full2=" << fullStep);
                        goto COMPUTE2;
                    }

                    for(size_t j=m;j>0;--j)
                    {
                        C3[j] = Cin[j] + k3[j] * halfStep;
                    }

                    compute(k3,C3,Ktop,xml);
                    mustSupport(C3,SubLevel);

                    if( rescale(fullStep,k3,C3) )
                    {
                        fullStep *= SAFE;
                        halfStep  = fullStep * half;
                        Y_XMLOG(xml, "full3=" << fullStep);
                        goto COMPUTE2;
                    }

                    for(size_t j=m;j>0;--j)
                    {
                        C4[j] = Cin[j] + k3[j] * fullStep;
                    }
                    mustSupport(C4,SubLevel);
                    compute(k4,C4,Ktop,xml);


                    
                    for(size_t j=m;j>0;--j)
                    {
                        xadd.free();
                        xadd << k1[j];
                        xadd << two * k2[j];
                        xadd << two * k3[j];
                        xadd << k4[j];
                        dC[j] = xadd.sum()/six;
                    }

                    if( rescale(fullStep,dC,Cin) )
                    {
                        fullStep *= SAFE;
                        halfStep  = fullStep * half;
                        Y_XMLOG(xml, "full4=" << fullStep);
                        goto COMPUTE2;
                    }

                    for(size_t j=m;j>0;--j)
                    {
                        Cex[j] = Cin[j] +  fullStep * dC[j];
                    }

                    Y_XMLOG(xml, "Cin=" << Cin);
                    Y_XMLOG(xml, "Cex=" << Cex);
                    Y_XMLOG(xml, "tau=" << real_t(fullStep));
                }

            }


#if 0
            void run(XWritable       &C,
                     const Level      L,
                     const XReadable &Ktop,
                     XMLog           &xml)
            {
                //--------------------------------------------------------------
                //
                //
                // first time check
                //
                //
                //--------------------------------------------------------------
                slacken(C, L, Ktop, xml);

                //--------------------------------------------------------------
                //
                //
                // initialize
                //
                //
                //--------------------------------------------------------------
                const size_t n = compile(C,L,Ktop,xml);
                switch(n)
                {
                    case 0: Y_XMLOG(xml, "[inactive]"); return;
                    case 1: Y_XMLOG(xml, "[singulet]");
                    {
                        const Prospect &pro = pps[1];
                        rcl.transfer(C, L, pro.cc, SubLevel);
                    } return;

                    default:
                        break;
                }
                const size_t  m    = nsp;
                const xreal_t SAFE = 0.9;
                const xreal_t one  = 1;
                xreal_t       tau  = 1;
                XArray        dC(nsp);

                rcl.transfer(Cin, SubLevel, C, L);  // as first compilation
                makeXdC(rho);                       // from first compilation
                makeJac();                          // from first compilation

                Y_XMLOG(xml, "Cin = " << Cin);
                Y_XMLOG(xml, "rho = " << rho);

                // rescale tau so that Cin+ tau*rho is OK
                if( rescale(tau, rho, Cin) ) {
                    tau *= SAFE;
                    Y_XMLOG(xml, "tau = " << real_t(tau) << " // from initial rates");
                }

            JSCAL:
                // rescale tau so that (I-tau*Jac) is invertible
                if( rescale(tau) )
                {
                    Y_XMLOG(xml, "tau = " << real_t(tau) << " // from initial jacobian");
                }


                //--------------------------------------------------------------
                //
                //
                // solve Euler's scheme for given tau
                //
                //
                //--------------------------------------------------------------

                for(size_t cycle=0;;++cycle)
                {
                    Y_XMLOG(xml, "[Newton #cyle=" << cycle << " tau=" << real_t(tau) << "]");
                    // get denominator
                    getDen(tau);

                    // compute guess forward step
                    for(size_t j=m;j>0;--j)
                    {
                        dC[j] = tau * rho[j];
                    }
                    std::cerr << "lhs = " << dC << std::endl;

                    // compute guess backward step
                    xlu.solve(Den, dC);
                    std::cerr << "dC  = " << dC  << std::endl;
                    std::cerr << "Cin = " << Cin << std::endl;

                    {
                        xreal_t fac = one;
                        if( rescale(fac, dC, Cin) )
                        {
                            // step is too big
                            std::cerr << "fac=" << fac << std::endl;
                            tau *= 0.5;

                            // recompute
                            rcl.transfer(Cin, SubLevel, C, L);
                            compute(rho, Cin, Ktop, xml);
                            goto JSCAL;


                        }


                        std::cerr << "ok=1" << std::endl;
                        for(size_t j=m;j>0;--j)
                        {
                            Cin[j] += dC[j];
                        }
                        std::cerr << "C" << cycle << " = " << Cin << std::endl;
                        compute(rho,Cin,Ktop,xml);
                    }

                    if(cycle>=20) break;
                }
            }
#endif



            const Cluster &        rcl; //!< persistent reference to cluster
            Aftermath              afm; //!< aftermath
            const size_t           neq; //!< rcl.size
            const size_t           nsp; //!< rcl.species.size
            XMatrix                ceq; //!< store all Ceq
            XMatrix                ddc; //!< store all delta Ceq
            XMatrix                Phi; //!< store derivatives
            XMatrix                Jac;
            XMatrix                Den;
            XArray                 Jdg;
            XArray                 Jxv;
            XArray                 Cin; //!< C input
            XArray                 Cws; //!< C workspace
            XArray                 Cex; //!< C exit
            XArray                 rho;
            Prospect::Series       pps; //!< running series
            XSeries                obj;
            CxxArray<XAdd,XMemory> xdc; //!< helper to compute delta C
            LU<xreal_t>            xlu; //!< linear algebra

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cracker);
        };

    }

}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(crack)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Species::Conc(C0,plexus.ran,0.3);

        Cracker crack(*cl);

        lib(std::cerr << "C0=","\t[",C0,"]");

        for(size_t iter=1;iter<=1;++iter)
        {
            crack.run(C0,TopLevel,K,plexus.xml);
            lib(std::cerr << "C1=","\t[",C0,"]");
        }





    }

    Y_SIZEOF(Prospect);

}
Y_UDONE()

