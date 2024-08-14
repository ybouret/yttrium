

#include "y/chemical/plexus.hpp"
#include "y/chemical/reactive/aftermath.hpp"

#include "y/utest/run.hpp"

#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"
#include "y/text/boolean.hpp"

#include "y/stream/libc/output.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {


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
            pps(neq),
            xdc(nsp,CopyOf,neq)
            {
            }


            virtual ~Cracker() noexcept
            {
            }

            //! remove all crucial
            void slacken(XWritable &       Ctop,
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
                    const Equilibrium &eq = **en; if( !eq.crucial(Ctop,TopLevel) ) continue;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = pps.size()+1;
                    XWritable         &cc = rcl.transfer(ceq[ii],SubLevel,Ctop,TopLevel);
                    XWritable         &dc = ddc[ii];
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);
                    if(Crucial!=st) throw Specific::Exception(eq.name.c_str(), "corrupted crucial state");

                    pps << Prospect(eq, eK, cc, afm.eval(dc, cc, SubLevel, Ctop, TopLevel, eq), dc);

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
                    rcl.transfer(Ctop, TopLevel, pro.cc, SubLevel);

                    goto DETECT_CRUCIAL;
                }
            }


            //! compile running prospect(s)
            size_t compile(XWritable &     Ctop,
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
                    XWritable         &cc = rcl.transfer(ceq[ii],SubLevel,Ctop,TopLevel);
                    XWritable         &dc = ddc[ii];
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked: continue;
                        case Crucial: throw Specific::Exception(eq.name.c_str(), "unexpected crucial state!");
                        case Running:
                            break;
                    }

                    const Prospect pro(eq, eK, cc, afm.eval(dc, cc, SubLevel, Ctop, TopLevel, eq), dc);
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


            

            void makePhi(const bool divided)
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
                    if(divided)
                    {
                        const xreal_t den = afm.xadd.dot(rcl.topology[ii],phi);
                        for(size_t j=m;j>0;--j) phi[j]/=den;
                    }
                }
            }

            void makeJac()
            {
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

                std::cerr << "Jdiag=[";
                for(size_t i=1;i<=m;++i)
                {
                    std::cerr << ' ' << real_t(Jac[i][i]);
                }
                std::cerr << "]" << std::endl;

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




            const Cluster &        rcl;
            Aftermath              afm;
            const size_t           neq;
            const size_t           nsp;
            XMatrix                ceq;
            XMatrix                ddc;
            XMatrix                Phi;
            XMatrix                Jac;
            Prospect::Series       pps;
            CxxArray<XAdd,XMemory> xdc;
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

        crack.slacken(C0,K,plexus.xml);
        crack.compile(C0,K,plexus.xml);
        lib(std::cerr << "C1=","\t[",C0,"]");

        crack.makePhi(false);
        std::cerr << "Phi0=" << crack.Phi << std::endl;
        
        crack.makePhi(true);
        std::cerr << "Phi1=" << crack.Phi << std::endl;
        crack.makeJac();
        std::cerr << "Jac =" << crack.Jac << std::endl;

    }

    Y_SIZEOF(Prospect);

}
Y_UDONE()

