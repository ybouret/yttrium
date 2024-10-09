
#include "y/chemical/plexus/device.hpp"

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
        increase(nspc),
        dof(mine.Nu.rows),
        ortho(nspc,dof),
        basis(dof)
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

    }

}




namespace Yttrium
{
    namespace Chemical
    {

        bool Device:: nullify(Ansatz &ans) noexcept
        {
            ans.dc.ld(0);
            ans.cc.ld(Cini);
            ans.ff = ff0;
            ans.xi = ans.ax = 0;
            return (ans.ok = false);
        }

        using namespace MKL;

        bool Device:: enhance(Ansatz &ans)
        {
            const xreal_t slope = aftermath.xadd.dot(ans.dc,gradient);
            if(slope.mantissa>=0.0)
            {
                //--------------------------------------------------------------
                //
                // numerically not satistfying
                //
                //--------------------------------------------------------------
                return nullify(ans);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // look for mininimum in [Cini:ans.cc]
                //
                //--------------------------------------------------------------
                Cend.ld(ans.cc);
                Device          &F  = *this;
                Triplet<xreal_t> xx = { 0,   -1,      1 };
                Triplet<xreal_t> ff = { ff0, -1, ans.ff };
                const xreal_t    xm = Minimize<xreal_t>::Locate(Minimizing::Inside, F, xx, ff);

                //--------------------------------------------------------------
                //
                // recompute ansatz from the minimum
                //
                //--------------------------------------------------------------
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

            ortho.free();
            basis.free();

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
            const size_t na = ansatz.size();
            {
                Y_XML_COMMENT(xml,"[Running]");

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

            // the first ansatz is the default choice


            //__________________________________________________________________
            //
            //
            //
            // build basis
            //
            //
            //__________________________________________________________________
            {
                Y_XML_SECTION(xml, "basis");
                assert( 0 == ortho.size );
                assert( 0 == basis.size );
                assert( na>1 );

                for(size_t i=1;i<=na;++i)
                {
                    const Ansatz        & ans = ansatz[i];
                    const Equilibrium   & eq  = ans.eq;
                    const Readable<int> & nu  = mine.iTopo[ eq.indx[SubLevel] ];
                    if( ortho.wouldAccept( nu ) )
                    {
                        Y_XMLOG(xml,ans);
                        if( (basis << ans).size >= dof )
                            break;
                    }
                }
                assert( basisOkWith(Cini,SubLevel) );
            }

            //__________________________________________________________________
            //
            //
            //
            // build ODE Steo
            //
            //
            //__________________________________________________________________
            {
                Y_XML_SECTION(xml, "ODE");
                
            }


            Y_DEVICE_RETURN(Locked);
        }

        bool Device:: basisOkWith(const XReadable &C, const Level L) const noexcept
        {
            for(const ANode *an=basis.head;an;an=an->next)
            {
                if( ! (**an).eq.canTolerate(C,L) ) return false;
            }
            return true;
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
