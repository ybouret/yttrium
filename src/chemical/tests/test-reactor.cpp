
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/container/cxx/series.hpp"

#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"
#include "y/text/boolean.hpp"

#include "y/stream/libc/output.hpp"

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




        class Reactor : public Quantized, public Counted
        {
        public:

            explicit Reactor(const Cluster &cl) :
            rcl(cl),
            afm(),
            ceq(rcl.size,rcl.species.size),
            ddc(rcl.size,rcl.species.size),
            pps(rcl.size),
            Cin(rcl.species.size),
            Cex(rcl.species.size),
            Cws(rcl.species.size),
            obj(rcl.size),
            xdc(rcl.species.size,CopyOf,rcl.size)
            {
            }

            virtual ~Reactor() noexcept {}


            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(size_t i=pps.size();i>0;--i) obj << pps[i].objectiveFunction(afm.xmul,C,L);
                return afm.xadd.normOf(obj);
            }

            xreal_t operator()(xreal_t u)
            {
                const xreal_t zero = 0;
                const xreal_t one  = 1;
                u               = Clamp(zero,u,one);      assert(u>=zero); assert(u<=one);
                const xreal_t v = Clamp(zero,one-u,one);  assert(v>=zero); assert(v<=one);

                for(size_t j=rcl.species.size;j>0;--j)
                {
                    xreal_t       cmin = Cin[j];
                    xreal_t       cmax = Cex[j];
                    const xreal_t c0   = cmin;
                    const xreal_t c1   = cmax;
                    if(cmax<cmin) Swap(cmin,cmax);
                    assert(cmin<=cmax);
                    Cws[j] = Clamp(cmin,c0*v+c1*u,cmax);
                }

                return objectiveFunction(Cws,SubLevel);
            }

            size_t compile(XWritable       &Ctop,
                           const XReadable &Ktop,
                           bool            &repl,
                           XMLog           &xml)
            {
                Y_XML_SECTION(xml, "Compile");
                size_t cycle = 0;
                repl         = false;
            EXAMINE:
                {
                    ++cycle;
                    Y_XML_SECTION_OPT(xml, "Examine", " cycle='" << cycle << "'");
                    pps.free();
                    for(const ENode *en=rcl.head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                        const size_t       ii = pps.size()+1;
                        XWritable         &cc = ceq[ii];
                        const Situation    st = afm.seek( rcl.transfer(cc,SubLevel,Ctop,TopLevel), SubLevel, eq, eK);
                        switch(st)
                        {
                            case Blocked: Y_XMLOG(xml, "(-) " << eq.name);
                                continue;

                            case Crucial: Y_XMLOG(xml, "(!) " << eq.name);
                                rcl.transfer(Ctop, TopLevel, cc, SubLevel);
                                repl = true;
                                goto EXAMINE;

                            case Running: Y_XMLOG(xml, "(+) " << eq.name);
                                break;
                        }
                        assert(Running==st);
                        XWritable     &dc = ddc[ii];
                        const xreal_t  xi = afm.eval(dc, cc, SubLevel, Ctop, TopLevel, eq);
                        const Prospect pro(eq,eK,cc,xi,dc);
                        pps << pro;
                    }
                }

                HeapSort::Call(pps,Prospect::CompareAX);

                const size_t npr = pps.size();
                for(size_t i=1;i<=npr;++i)
                {
                    const Prospect &pro = pps[i];
                    pro.eq.mustSupport(Ctop,TopLevel);
                    for(size_t j=npr;j>0;--j)
                    {
                        pro.eq.mustSupport(pps[j].cc,SubLevel);
                    }
                    if(xml.verbose)
                    {
                        pro.show(std::cerr << "ff=" << std::setw(15) << real_t( objectiveFunction(pro.cc, SubLevel)) << "|", rcl, true) << std::endl;
                    }
                }
                if(!repl)
                {
                    Y_XMLOG(xml,"ftop=" << std::setw(15) << real_t( objectiveFunction(Ctop, TopLevel)) );
                }


                return pps.size();
            }


            void promote(XWritable       &Ctop,
                         const XReadable &Ktop,
                         XMLog           &xml)
            {
                Y_XML_SECTION(xml, "Promote");

                // compile
                while(true)
                {
                    bool         repl = false;
                    const size_t npro = compile(Ctop, Ktop, repl, xml);
                    switch(npro)
                    {
                        case 0: Y_XMLOG(xml, "[Inactive]"); return;
                        case 1:
                            if(!repl)
                            {
                                rcl.transfer(Ctop, TopLevel, pps[1].cc, SubLevel);
                            }
                            return;
                        default:
                            break;
                    }
                    assert(npro>1);
                    if(repl) continue;
                    break;
                }

                // initialize input
                rcl.transfer(Cin, SubLevel, Ctop, TopLevel);

                // compute xdc
                const size_t m = rcl.species.size;
                const size_t n = pps.size();
                for(size_t j=m;j>0;--j)
                {
                    xdc[j].free();
                }

                for(size_t i=n;i>0;--i)
                {
                    const Prospect  &pro = pps[i];
                    const XReadable &dc  = pro.dc;
                    for(size_t j=m;j>0;--j)
                        xdc[j] << dc[j];
                }

                // compute dC into Cws
                const xreal_t zero  = 0;
                const xreal_t one   = 1;
                bool          abate = false;
                xreal_t       scale = one;


                for(size_t j=m;j>0;--j)
                {
                    const real_t dc = Cws[j] = xdc[j].sum();
                    if(dc<zero)
                    {
                        const xreal_t cc = Cin[j]; assert(cc>zero);
                        const xreal_t sc = cc / (-dc);
                        if(!abate)
                        {
                            abate = true;
                            scale = sc;
                        }
                        else
                        {
                            scale = Min(scale,sc);
                        }
                    }
                }

                std::cerr << "dC    = " << Cws << std::endl;
                Y_XMLOG(xml, "abate = " << BooleanTo::text(abate) );
                Y_XMLOG(xml, "scale = " << real_t(scale));

                if(abate) {
                    scale *= 0.999;
                }

                for(size_t j=m;j>0;--j)
                {
                    Cex[j] = Cin[j] + scale * Cws[j];
                }


                {
                    Reactor &F = *this;
                    OutputFile   fp("promote.dat");
                    const size_t np = 1000;
                    for(size_t j=0;j<=np;++j)
                    {
                        const real_t u = double(j)/np;
                        fp("%.15g %.15g\n", u, real_t(F(u)));
                    }
                }

            }



            const Cluster &        rcl;
            Aftermath              afm;
            XMatrix                ceq;
            XMatrix                ddc;
            Prospect::Series       pps;
            XArray                 Cin;
            XArray                 Cex;
            XArray                 Cws;
            XSeries                obj; //!< store objective function
            CxxArray<XAdd,XMemory> xdc;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };
    }

}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(reactor)
{
    Weasel::Compiler  &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool           verbose = true;
    XMLog          xml(verbose);
    SharedClusters   cls = new Clusters(eqs,xml);
    const XReadable &K   = cls->K(0);


    XVector C0(lib->size(),0);

    for(const Cluster *cl=(*cls)->head;cl;cl=cl->next)
    {
        Reactor rxn(*cl);
        Species::Conc(C0,ran,0.3);

        lib(std::cerr << "C0=","\t[",C0,"]");
        //rxn.compile(C0,K,repl,xml);
        rxn.promote(C0,K,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");

    }

    Y_SIZEOF(Prospect);
    Y_SIZEOF(Reactor);

}
Y_UDONE()

