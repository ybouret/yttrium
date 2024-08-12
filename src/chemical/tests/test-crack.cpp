

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
            add(rcl.size),
            ceq(rcl.size,rcl.species.size),
            ddc(rcl.size,rcl.species.size),
            pps(rcl.size),
            Cin(rcl.species.size),
            Cmd(rcl.species.size),
            Cex(rcl.species.size),
            Cws(rcl.species.size),
            Cst(rcl.species.size),
            rk1(rcl.species.size),
            rk2(rcl.species.size),
            obj(rcl.size),
            xdc(rcl.species.size,CopyOf,rcl.size)
            {
            }

            virtual ~Cracker() noexcept {}



            size_t compile(XWritable       &C,
                           const Level      L,
                           const XReadable &Ktop,
                           bool            &repl,
                           XMLog           &xml)
            {
                Y_XML_SECTION(xml, "compile");
                size_t cycle = 0;
                repl         = false;
            EXAMINE:
                {
                    ++cycle;
                    Y_XML_SECTION_OPT(xml, "examine", " cycle='" << cycle << "'");
                    pps.free();
                    add.free();
                    for(const ENode *en=rcl.head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                        const size_t       ii = pps.size()+1;
                        XWritable         &cc = ceq[ii];
                        const Situation    st = afm.seek( rcl.transfer(cc,SubLevel,C,L), SubLevel, eq, eK);
                        switch(st)
                        {
                            case Blocked: Y_XMLOG(xml, "(-) " << eq.name);
                                continue;

                            case Crucial: Y_XMLOG(xml, "(!) " << eq.name << " @xi=" << real_t(afm.eval( cc, SubLevel, C, L, eq)) );
                                rcl.transfer(C, L, cc, SubLevel);
                                repl = true;
                                goto EXAMINE;

                            case Running: Y_XMLOG(xml, "(+) " << eq.name);
                                break;
                        }
                        assert(Running==st);
                        XWritable     &dc = ddc[ii];
                        const xreal_t  xi = afm.eval(dc, cc, SubLevel, C, L, eq);
                        const Prospect pro(eq,eK,cc,xi,dc);
                        pps << pro;
                        add << pro.ax;
                    }
                }

                const size_t n = pps.size();


                HeapSort::Call(pps,Prospect::CompareAX);
                for(size_t i=1;i<=n;++i)
                {
                    const Prospect &pro = pps[i];
                    pro.eq.mustSupport(C,L);
                    for(size_t j=n;j>0;--j)
                    {
                        pro.eq.mustSupport(pps[j].cc,SubLevel);
                    }
                    if(xml.verbose)
                    {
                        pro.show(xml(),rcl,true) << std::endl;
                    }
                }

                return n;
            }

            // post compile computation
            void getXdC(XWritable &dSub)
            {
                const size_t n = pps.size();
                const size_t m = rcl.species.size;
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

                for(size_t j=m;j>0;--j)
                {
                    dSub[j] = xdc[j].sum();
                }
            }


            void evaluate(XWritable       &Ctop,
                          const XReadable &Ktop,
                          XMLog           &xml)
            {



                bool         repl = false;
                const size_t n    = compile(Ctop, TopLevel, Ktop, repl, xml);


                //--------------------------------------------------------------
                //
                //
                // compute increase and scale
                //
                //
                //--------------------------------------------------------------
                rcl.transfer(Cin, SubLevel, Ctop, TopLevel);
                getXdC(rk1);
                const xreal_t zero  = 0;
                const xreal_t one   = 1;
                const xreal_t hmax  = 2;
                bool          cut   = false;
                xreal_t       h     = one;
                const size_t  m     = rcl.species.size;

                for(size_t j=m;j>0;--j)
                {
                    const real_t dc = rk1[j];
                    if(dc<zero)
                    {
                        const xreal_t cc = Cin[j]; assert(cc>zero);
                        const xreal_t sc = cc / (-dc);
                        if(!cut)
                        {
                            cut = true;
                            h   = sc;
                        }
                        else
                        {
                            h = Min(h,sc);
                        }
                    }
                }
                Y_XMLOG(xml, "h     = " << real_t(h));
                if(cut)
                {
                    h *= 0.9;
                    if(h>hmax) h=hmax;
                }

                Y_XMLOG(xml, "rk1   = " << rk1);
                Y_XMLOG(xml, "cut   = " << BooleanTo::text(cut) );
                if(cut)
                    Y_XMLOG(xml, "h     = " << real_t(h));

                {
                    const real_t half = h * 0.5;
                    for(size_t j=m;j>0;--j)
                    {
                        Cmd[j] = Cin[j] + half * rk1[j];
                    }
                }
                Y_XMLOG(xml, "Cmd   = " << Cmd);

                repl = false;
                if( n != compile(Cmd, SubLevel, Ktop, repl, xml) ) throw Specific::Exception("here", "changed status");
                if( repl ) throw Specific::Exception("here","unexepected replaced");

                getXdC(rk2);
                Y_XMLOG(xml, "Cin   = " << Cin);
                Y_XMLOG(xml, "rk2   = " << rk2);




            }

            

            const Cluster &        rcl;
            Aftermath              afm;
            XAdd                   add;
            XMatrix                ceq;
            XMatrix                ddc;
            Prospect::Series       pps;
            XArray                 Cin;
            XArray                 Cmd;
            XArray                 Cex;
            XArray                 Cws;
            XArray                 Cst;
            XArray                 rk1;
            XArray                 rk2;
            XSeries                obj; //!< store objective function
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
        Species::Conc(C0,ran,0.3);



        lib(std::cerr << "C0=","\t[",C0,"]");

        Cracker crack(*cl);
        crack.evaluate( C0, K, xml);



        lib(std::cerr << "C1=","\t[",C0,"]");

    }

    Y_SIZEOF(Prospect);
    Y_SIZEOF(Cracker);

}
Y_UDONE()

