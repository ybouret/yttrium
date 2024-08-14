

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

                    goto DETECT_CRUCIAL;
                }
            }


            //! compile running prospect(s)
            size_t compile(const XReadable & C,
                           const Level       L,
                           const XReadable & Ktop,
                           XMLog &           xml)
            {
                Y_XML_SECTION(xml, "Compile");
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
                    for(size_t j=n;j>0;--j)
                    {
                        pro.eq.mustSupport(pps[j].cc,SubLevel);
                    }
                }

                return n;
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


            void compute(XWritable &dC)
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

            void run(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml)
            {
                // first time check
                slacken(C, L, Ktop, xml);

                // initialize
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

                // full step ?
                const xreal_t zero = 0;
                const xreal_t one  = 1;
                const size_t  m    = nsp;
                rcl.transfer(Cin, SubLevel, C, L); // Cin@SubLevel
                compute(rho);                      // rho@SubLevel
                xreal_t tau = 1;

                // initial scaling
                {
                    bool    abate = false;
                    xreal_t scale = 1;
                    for(size_t j=m;j>0;--j)
                    {
                        const xreal_t d = rho[j];
                        if(d<zero)
                        {
                            const xreal_t c = Cin[j]; assert(Cin[j]>zero);
                            const xreal_t s = c/(-d);
                            if(s<scale)
                            {
                                abate = true;
                                scale = s;
                            }
                        }
                    }

                    Y_XMLOG(xml, "abate = " << BooleanTo::text(abate));
                    Y_XMLOG(xml, "scale = " << real_t(scale));
                    if(abate)
                    {
                        scale *= 0.9;
                        tau *= scale;
                        Y_XMLOG(xml, "scale = " << real_t(scale));
                        Y_XMLOG(xml, "tau   = " << real_t(tau));
                    }
                }

                // compute jacobian and rescale initil tau
                makeJac();
                {
                JSCALE:
                    for(size_t j=m;j>0;--j)
                    {
                        const xreal_t dg = (one-tau*Jdg[j]).abs();
                        const xreal_t xv = tau*Jxv[j];
                        if(dg<=xv) {
                            --Coerce(tau.exponent);
                            Y_XMLOG(xml, "tau   = " << real_t(tau));
                            goto JSCALE;
                        }
                    }
                }

            DEN:
                for(size_t i=m;i>0;--i)
                {
                    for(size_t j=m;j>0;--j)
                    {
                        Den[i][j] = - tau * Jac[i][j];
                    }
                    Den[i][i] += one;
                    Cws[i]     = tau * rho[i];
                }

                if(!xlu.build(Den))
                {
                    throw Specific::Exception("here","unexpected singular Den");
                }

                xlu.solve(Den,Cws);
                Y_XMLOG(xml, "Cws=" << Cws);
                for(size_t j=m;j>0;--j)
                {
                    const xreal_t d = Cws[j];
                    if(d<zero)
                    {
                        if( -d >= Cin[j] )
                        {
                            Y_XMLOG(xml, "[shrink!]");
                            --Coerce(tau.exponent);
                            goto DEN;
                        }
                    }
                }

                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + Cws[j];
                }
                for(const SNode *sn=rcl.species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    const size_t j = sp.indx[SubLevel];
                    Y_XMLOG(xml, std::setw(15) << real_t(Cin[j]) << " -> " << std::setw(15) << real_t(Cex[j]) << " @" << sp);
                }




            }




            const Cluster &        rcl;
            Aftermath              afm;
            const size_t           neq;
            const size_t           nsp;
            XMatrix                ceq;
            XMatrix                ddc;
            XMatrix                Phi;
            XMatrix                Jac;
            XMatrix                Den;
            XArray                 Jdg;
            XArray                 Jxv;
            XArray                 Cin;
            XArray                 Cws;
            XArray                 Cex;
            XArray                 rho;
            Prospect::Series       pps;
            CxxArray<XAdd,XMemory> xdc;
            LU<xreal_t>            xlu;

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

        crack.run(C0,TopLevel,K,plexus.xml);

        lib(std::cerr << "C1=","\t[",C0,"]");






    }

    Y_SIZEOF(Prospect);

}
Y_UDONE()

