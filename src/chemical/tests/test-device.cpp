#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"
#include "y/utest/run.hpp"
#include "y/jive/module.hpp"

#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Ansatz
        {
        public:
            typedef CxxSeries<Ansatz,XMemory> Series;

            Ansatz(const Equilibrium &_eq,
                   const xreal_t      _ek,
                   const Situation    _st,
                   XWritable &        _cc,
                   const xreal_t      _xi,
                   XWritable &        _dc) noexcept;

            ~Ansatz() noexcept;
            Ansatz(const Ansatz &) noexcept;

            Y_OSTREAM_PROTO(Ansatz);

            static SignType DecreasingAX(const Ansatz &lhs, const Ansatz &rhs) noexcept;

            xreal_t objectiveFunction(XMul &X, const XReadable &C, const Level L) const;

            const Equilibrium &eq;
            const xreal_t      ek;
            const Situation    st;
            XWritable &        cc; //!< optimal concentrations
            xreal_t            xi; //!< corresponding extent from Cini
            xreal_t            ax; //!< |xi|
            XWritable &        dc; //!< corresponding delta fron Cini
            xreal_t            ff;

        private:
            Y_DISABLE_ASSIGN(Ansatz);

        };

        xreal_t Ansatz:: objectiveFunction(XMul &X, const XReadable &C, const Level L) const
        {
            return eq.affinity(ek, X, C, L);
        }

        std::ostream & operator<<(std::ostream &os, const Ansatz &ans)
        {

            os << "|ff=" << Formatted::Get("%15.4g",real_t(ans.ff));
            os << "|xi=" << std::setw(15) << real_t(ans.xi);
            os << " @" << ans.eq;
            return os;
        }


        SignType Ansatz:: DecreasingAX(const Ansatz &lhs, const Ansatz &rhs) noexcept
        {
            assert(Crucial==lhs.st);
            assert(Crucial==rhs.st);
            return Sign::Of(rhs.ax,lhs.ax);
        }

        Ansatz:: Ansatz(const Equilibrium &_eq,
                        const xreal_t      _ek,
                        const Situation    _st,
                        XWritable &        _cc,
                        const xreal_t      _xi,
                        XWritable &        _dc) noexcept :
        eq(_eq),
        ek(_ek),
        st(_st),
        cc(_cc),
        xi(_xi),
        ax(xi.abs()),
        dc(_dc),
        ff(0.0)
        {
        }

        Ansatz:: Ansatz(const Ansatz &_) noexcept :
        eq(_.eq),
        ek(_.ek),
        st(_.st),
        cc(_.cc),
        xi(_.xi),
        ax(_.ax),
        dc(_.dc),
        ff(_.ff)
        {

        }

        Ansatz:: ~Ansatz() noexcept {}


        class Device : public Joint
        {
        public:
            explicit Device(const Cluster &);
            virtual ~Device() noexcept;


            void process(XWritable       &C,
                         const Level      L,
                         const XReadable &K,
                         XMLog           &xml);

            void showAnsatz(XMLog &xml) const;

            xreal_t objectiveFunction(const XReadable &C, const Level L);

            Aftermath      aftermath;
            XMatrix        EqConc;
            XMatrix        EqDiff;
            Ansatz::Series ansatz;
            XSeries        objFcn;
            XArray         Cini;
            XArray         Cend;
            XArray         Ctmp;
            xreal_t        Fini;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Device);
        };

        Device:: ~Device() noexcept {}

        Device:: Device(const Cluster &_) :
        Joint(_),
        aftermath(),
        EqConc(neqs,nspc),
        EqDiff(neqs,nspc),
        ansatz(neqs),
        objFcn(neqs),
        Cini(nspc),
        Cend(nspc),
        Ctmp(nspc),
        Fini()
        {
        }

        xreal_t Device:: objectiveFunction(const XReadable &C, const Level L)
        {
            objFcn.free();
            XMul &X = aftermath.xmul;
            for(size_t i=ansatz.size();i>0;--i)
                objFcn << ansatz[i].objectiveFunction(X,C,L);
            return aftermath.xadd.normOf(objFcn);
        }


        void Device:: showAnsatz(XMLog &xml) const
        {
            if(!xml.verbose) return;
            const size_t na = ansatz.size();
            Y_XML_SECTION_OPT(xml, "Ansatz", "count=" << ansatz.size() );
            for(size_t i=1;i<=na;++i)
            {
                xml() << ansatz[i] << std::endl;
            }
        }


        void Device:: process(XWritable       &C,
                              const Level      L,
                              const XReadable &K,
                              XMLog           &xml)
        {
            static const char fn[] = "Chemical::Device::process";

            Y_XML_SECTION(xml, "process");

            //__________________________________________________________________
            //
            //
            //
            // Moving to a fully safe place
            //
            //
            //__________________________________________________________________
            {
                size_t cycle = 0;
                while(true)
                {

                    //__________________________________________________________
                    //
                    //
                    // detect situation
                    //
                    //__________________________________________________________
                    ++cycle;
                    Y_XML_COMMENT(xml, "cycle #" << cycle);
                    ansatz.free();
                    bool crucial = false;
                    for(const ENode *en=mine.head;en;en=en->next)
                    {
                        const size_t       ii = 1+ansatz.size();
                        const Equilibrium &eq = **en;
                        const xreal_t      ek = K[eq.indx[TopLevel]];
                        XWritable         &cc = mine.transfer(EqConc[ii],SubLevel,C, L);
                        XWritable         &dc = EqDiff[ii];
                        const Situation    st = aftermath.seek(cc,SubLevel,eq,ek);

                        switch(st)
                        {
                            case Blocked: Y_XMLOG(xml, "[Blocked] " << eq);
                                continue;

                            case Crucial: Y_XMLOG(xml, "[Crucial] " << eq);
                                crucial = true;
                                break;

                            case Running: if(crucial) continue;
                                Y_XMLOG(xml, "[Running] " << eq);
                                break;
                        }

                        const xreal_t xi = aftermath.eval(dc, cc, SubLevel, C, L, eq);
                        const Ansatz  ans(eq,ek,st,cc,xi,dc);
                        ansatz << ans;
                    }
                    if(!crucial) break; // will process all running

                    //__________________________________________________________
                    //
                    //
                    // remove running among crucial
                    //
                    //__________________________________________________________
                    assert( ansatz.size() > 0 );
                    for(size_t i = ansatz.size(); i>0;--i )
                    {
                        Ansatz &ans = ansatz[i];
                        switch(ans.st)
                        {
                            case Blocked: // shouldn't get here
                                throw Specific::Exception(fn, "forbidden Blocked %s", ans.eq.name.c_str());

                            case Crucial: // keep
                                continue;

                            case Running: // remove
                                ansatz.remove(i);
                                continue;
                        }
                    }

                    assert(ansatz.size()>0);

                    //__________________________________________________________
                    //
                    //
                    // order by decreasing |xi|
                    //
                    //__________________________________________________________
                    HeapSort::Call(ansatz, Ansatz::DecreasingAX);
                    Y_XML_COMMENT(xml, "[Crucial]");
                    showAnsatz(xml);

                    //__________________________________________________________
                    //
                    //
                    // set new starting points
                    //
                    //__________________________________________________________
                    {
                        const Ansatz &ans = ansatz.head();
                        mine.transfer(C, L, ans.cc, SubLevel);
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            //
            // Sorting out Running ansatz
            //
            //
            //__________________________________________________________________
            {
                Y_XML_COMMENT(xml,"[Running]");
                {
                    Y_XML_SECTION(xml, "Initial Condition");
                    mine.transfer(Cini, SubLevel,C,L);
                    Fini = objectiveFunction(Cini,SubLevel);
                    Y_XMLOG(xml, " FF=" << Formatted::Get("%15.4g",real_t(Fini)));
                }
                for(size_t i=ansatz.size();i>0;--i)
                {
                    Ansatz &ans = ansatz[i];
                    ans.eq.mustSupport(Cini,   SubLevel);
                    ans.eq.mustSupport(ans.cc, SubLevel);
                    ans.ff = objectiveFunction(ans.cc,SubLevel);
                }
                showAnsatz(xml);
            }

        }

    }
}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(device)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    XRealOutput::Mode = XRealOutput::Compact;

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

        Device dev(*cl);

        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.1);
            lib(std::cerr << "C0=","\t[",C0,"]");
            ward(C0,dC,TopLevel,xml);
            lib(std::cerr << "C0=","\t[",C0,"]");

            dev.process(C0, TopLevel, K, xml);
        }

    }
}
Y_UDONE()
