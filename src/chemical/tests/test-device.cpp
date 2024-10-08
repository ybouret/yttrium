#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"
#include "y/utest/run.hpp"
#include "y/jive/module.hpp"

#include "y/chemical/reactive/aftermath.hpp"
#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"

#include "y/stream/libc/output.hpp"
#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"

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

            static SignType DecreasingAX(const Ansatz &lhs, const Ansatz &rhs) noexcept;         //!< decreasing |xi| for Crucial
            static SignType IncreasingFF(const Ansatz &lhs, const Ansatz &rhs) noexcept;         //!< increasing ff
            xreal_t         objectiveFunction(XMul &X, const XReadable &C, const Level L) const; //!< affinity

            const Equilibrium &eq;
            const xreal_t      ek;
            const Situation    st;
            XWritable &        cc; //!< optimal concentrations
            xreal_t            xi; //!< corresponding extent from Cini
            xreal_t            ax; //!< |xi|
            XWritable &        dc; //!< corresponding delta fron Cini
            xreal_t            ff;
            bool               ok; //!< decrease

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

        SignType Ansatz:: IncreasingFF(const Ansatz &lhs, const Ansatz &rhs) noexcept
        {
            assert(Running==lhs.st); assert(lhs.ff.mantissa>=0);
            assert(Running==rhs.st); assert(rhs.ff.mantissa>=0);
            return Sign::Of(lhs.ff,rhs.ff);
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
        ff(0.0),
        ok(false)
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
        ff(_.ff),
        ok(_.ok)
        {

        }

        Ansatz:: ~Ansatz() noexcept {}

    }

}


#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {

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
            xreal_t objectiveGradient(const XReadable &C, const Level L);
            xreal_t operator()(const xreal_t u); //!< objFunc @Ctmp  = Cini(1-u)+Cend*u u in [0:1]
            bool    enhance(Ansatz &);
            bool    nullify(Ansatz &) noexcept; //!< nullify and return false

            Aftermath      aftermath;
            XMatrix        EqConc;
            XMatrix        EqDiff;
            Ansatz::Series ansatz;
            XArray         Cini;
            XArray         Cend;
            XArray         Ctmp;
            xreal_t        ff0;      //!< objective function at Cini
            XSeries        objValue; //!< to compute objective function
            XArray         gradient; //!< gradient
            XSwell         increase; //!< to help compute gradient


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
        Cini(nspc),
        Cend(nspc),
        Ctmp(nspc),
        ff0(),
        objValue(neqs),
        gradient(nspc),
        increase(nspc)
        {
        }

        xreal_t Device:: objectiveFunction(const XReadable &C, const Level L)
        {
            objValue.free();
            XMul &X = aftermath.xmul;
            for(size_t i=ansatz.size();i>0;--i)
                objValue << ansatz[i].objectiveFunction(X,C,L);
            return aftermath.xadd.normOf(objValue);
        }

        xreal_t Device:: operator()(const xreal_t u)
        {
            const xreal_t one(1);
            const xreal_t v = one-u;
            for(size_t i=nspc;i>0;--i)
            {
                const xreal_t c0 = Cini[i];
                const xreal_t c1 = Cend[i];
                xreal_t cmin = c0, cmax = c1;
                if(cmax<cmin) Swap(cmin,cmax);
                Ctmp[i] = Clamp(cmin, v*c0 + u * c1, cmax);
            }
            return objectiveFunction(Ctmp,SubLevel);
        }

        xreal_t Device:: objectiveGradient(const XReadable &C,
                                           const Level      L)
        {
            const xreal_t _0;
            increase.forEach( &XAdd::free );
            objValue.free();
            gradient.ld(_0);


            const size_t m  = nspc;

            {
                XWritable & dA = Ctmp;
                XMul      &  X = aftermath.xmul;
                for(size_t i = ansatz.size();i>0;--i)
                {
                    const Ansatz      &ans = ansatz[i];
                    const Equilibrium &eq  = ans.eq;
                    const xreal_t      A   = ans.objectiveFunction(X,C,L);
                    eq.drvsAffinity(dA, SubLevel, C, L);
                    for(size_t j=m;j>0;--j)
                    {
                        increase[j] << A * dA[j];
                    }
                    objValue << A;
                }
            }

            const xreal_t den = aftermath.xadd.normOf(objValue);
            if(den.mantissa>0)
            {
                for(size_t j=m;j>0;--j)
                {
                    gradient[j] = increase[j].sum() / den;
                }
            }

            return den;
        }


        void Device:: showAnsatz(XMLog &xml) const
        {
            if(!xml.verbose) return;
            const size_t na = ansatz.size();
            Y_XML_SECTION_OPT(xml, "Ansatz", "count=" << ansatz.size() << "/" << neqs);
            for(size_t i=1;i<=na;++i)
            {
                xml() << ansatz[i] << std::endl;
            }
        }


        using namespace MKL;


        bool Device:: nullify(Ansatz &ans) noexcept
        {
            ans.dc.ld(0);
            ans.cc.ld(Cini);
            ans.ff = ff0;
            ans.xi = ans.ax = 0;
            return (ans.ok = false);
        }

        bool Device:: enhance(Ansatz &ans)
        {
            const xreal_t slope = aftermath.xadd.dot(ans.dc,gradient);
            if(slope.mantissa>=0.0)
            {
                // numerically not satistfying
                return nullify(ans);
            }
            else
            {
                // look for mininimum in [Cini:ans.cc]
                Cend.ld(ans.cc);
                Device          &F  = *this;
                Triplet<xreal_t> xx = { 0,   -1,      1 };
                Triplet<xreal_t> ff = { ff0, -1, ans.ff };
                const xreal_t    xm = Minimize<xreal_t>::Locate(Minimizing::Inside, F, xx, ff);

                // recompute ansatz
                ans.ff = F(xm); if(ans.ff>ff0) return nullify(ans);
                ans.cc.ld(Ctmp);
                ans.xi = aftermath.eval(ans.dc, ans.cc, SubLevel, Cini, SubLevel,ans.eq);
                ans.ax = ans.xi.abs();
                return (ans.ok = true);
            }
        }


        void Device:: process(XWritable       &C,
                              const Level      L,
                              const XReadable &K,
                              XMLog           &xml)
        {
            static const char fn[] = "Chemical::Device::process";

            Y_XML_SECTION(xml, "process");
            Jive::VirtualFileSystem::TryRemove(LocalFS::Instance(), ".", "pro", VFS::Entry::Ext);

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

                const size_t na = ansatz.size();
                switch(na)
                {
                    case 0:
                        Y_XMLOG(xml, "[[Jammed]]");
                        return;

                    case 1:
                        Y_XMLOG(xml, "[[Solved]]/Single");
                        mine.transfer(C,L,ansatz[1].cc, SubLevel);
                        return;

                    default:
                        break;
                }

                //______________________________________________________________
                //
                //
                // set common initial conditions
                //
                //______________________________________________________________
                {
                    Y_XML_SECTION(xml, "Initial Conditions");
                    mine.transfer(Cini, SubLevel,C,L);
                    ff0 = objectiveGradient(Cini,SubLevel);
                    Y_XMLOG(xml, " ff=" << Formatted::Get("%15.4g",real_t(ff0)) << " (" << ff0 << "/" << objectiveFunction(Cini,SubLevel) << ")");
                }

                size_t good = 0;
                for(size_t i=ansatz.size();i>0;--i)
                {
                    Ansatz &ans = ansatz[i]; assert(Running==ans.st);
                    ans.eq.mustSupport(Cini,   SubLevel);
                    ans.eq.mustSupport(ans.cc, SubLevel);
                    ans.ff = objectiveFunction(ans.cc,SubLevel);

                    if(false)
                    {
                        Cend.ld(ans.cc);
                        const String fileName = ans.eq.fileName() + ".pro";
                        OutputFile   fp(fileName);
                        const size_t np = 1000;
                        for(size_t i=0;i<=np;++i)
                        {
                            const double u = double(i)/np;
                            const double f = double( (*this)(u) );
                            fp("%.15g %.15g\n", u, f);
                        }
                        const xreal_t slope = aftermath.xadd.dot(ans.dc,gradient);
                        std::cerr << "plot '" << fileName << "'  w l, " << real_t(ff0) << "+(" << real_t(slope) << ")*x" << std::endl;
                    }

                    if( enhance(ans) )
                    {
                        Y_XMLOG(xml, "(+) " << ans);
                        ++good;
                    }
                    else
                    {
                        Y_XMLOG(xml, "(-) " << ans);
                    }
                }
                Y_XML_COMMENT(xml, "good=" << good << "/" << ansatz.size() << "/" << neqs);
                HeapSort::Call(ansatz,Ansatz::IncreasingFF);
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
