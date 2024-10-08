#include "y/chemical/plexus/device.hpp"
#include "y/chemical/plexus/wardens.hpp"

#include "y/chemical/plexus.hpp"


#include "y/utest/run.hpp"
#include "y/jive/module.hpp"


#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"

#include "y/stream/libc/output.hpp"
#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"





#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {

       

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
                ans.ff = F(xm); if(ans.ff>=ff0) return nullify(ans);
                ans.cc.ld(Ctmp);
                ans.xi = aftermath.eval(ans.dc, ans.cc, SubLevel, Cini, SubLevel,ans.eq);
                ans.ax = ans.xi.abs();
                return (ans.ok = true);
            }
        }

#define Y_DEVICE_RETURN(RES) do { Y_XMLOG(xml,"[[" << #RES << "]]"); return RES; } while(false)

        Outcome Device:: process(XWritable       &C,
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
                size_t makeSafer = 0;
                while(true)
                {

                    //__________________________________________________________
                    //
                    //
                    // detect situation
                    //
                    //__________________________________________________________
                    ++makeSafer;
                    Y_XML_COMMENT(xml, "makeSafer #" << makeSafer);
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

                            case Running: if(crucial) continue; // won't store any more running
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
                    // set new starting phase space and check again
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
                        Y_DEVICE_RETURN(Jammed);

                    case 1:
                        mine.transfer(C,L,ansatz[1].cc, SubLevel);
                        Y_DEVICE_RETURN(Solved);

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
                for(size_t i=na;i>0;--i)
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
                        // Y_XMLOG(xml, "(+) " << ans);
                        ++good;
                    }
                    else
                    {
                        // Y_XMLOG(xml, "(-) " << ans);
                    }
                }
                Y_XML_COMMENT(xml, "good=" << good << "/" << ansatz.size() << "/" << neqs);
                HeapSort::Call(ansatz,Ansatz::IncreasingFF);
                showAnsatz(xml);
            }


            Y_DEVICE_RETURN(Locked);
        }


        void Device:: computeRate(XWritable &rate)
        {

            //------------------------------------------------------------------
            //
            // initialize inc
            //
            //------------------------------------------------------------------
            increase.forEach( &XAdd::free );

            //------------------------------------------------------------------
            //
            // use prospects to compute increases
            //
            //------------------------------------------------------------------
            for(size_t i=ansatz.size();i>0;--i)
            {
                const Ansatz &ans = ansatz[i];
                if(!ans.ok) continue; //
                ans.step(increase);
            }

            //------------------------------------------------------------------
            //
            // deduce rate
            //
            //------------------------------------------------------------------
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const size_t j = (**sn).indx[ SubLevel ];
                rate[j] = increase[j].sum();
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
