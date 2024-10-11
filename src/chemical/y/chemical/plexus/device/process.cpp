#include "y/chemical/plexus/device.hpp"

#include "y/stream/libc/output.hpp"
#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {



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
#           include "crucial.hxx"

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

                    case 1: {
                        const Ansatz &ans = ansatz[1];
                        mine.transfer(C,L,ans.cc, SubLevel);
                        basis << ans;
                        assert(1==basis.size);
                    } Y_DEVICE_RETURN(Solved);

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
                    ff0 = ff1 = objectiveGradient(Cini,SubLevel);
                    Y_XMLOG(xml, " ff=" << Formatted::Get("%15.4g",real_t(ff0)) << " (" << ff0 << "/" << objectiveFunction(Cini,SubLevel) << ")");
                    assert(ff0.mantissa>=0.0);
                }

                assert(na>=2);
                assert(ff0.mantissa>0.0);
                assert(ff1.mantissa>0.0);


                size_t good = 0;
                for(size_t i=na;i>0;--i)
                {
                    Ansatz &ans = ansatz[i]; assert(Running==ans.st);
                    ans.eq.mustSupport(Cini,   SubLevel);
                    ans.eq.mustSupport(ans.cc, SubLevel);
                    ans.ff = objectiveFunction(ans.cc,SubLevel);

                    if( enhance(ans) )
                    {
                        ++good;
                    }

                }
                Y_XML_COMMENT(xml, "good=" << good << "/" << ansatz.size() << "/" << neqs);
                HeapSort::Call(ansatz,Ansatz::IncreasingFF);
                showAnsatz(xml);

                // the first ansatz is the default choice
                Cend.ld(ansatz[1].cc); saveProfile("min.pro",1000);
            }


            //__________________________________________________________________
            //
            //
            //
            // Initialize first guess, postpone ff1=0 detection to always
            // build basis
            //
            //__________________________________________________________________
            const Ansatz &Aopt = ansatz[1];
            ff1 = Aopt.ff;
            Copt.ld(Aopt.cc);


            //__________________________________________________________________
            //
            //
            //
            // build basis in any case for asymptotic method
            //
            //
            //__________________________________________________________________
#           include "mkbasis.hxx"


            //__________________________________________________________________
            //
            //
            //
            // and now test if the first Ansatz is solving
            //
            //
            //__________________________________________________________________
            if(ff1.mantissa<=0)
            {
                Y_XML_COMMENT(xml, "solving by " << Aopt.eq);
                mine.transfer(C,L,Copt,SubLevel);
                Y_DEVICE_RETURN(Solved);
            }
            assert(ff1.mantissa>0.0);

            //__________________________________________________________________
            //
            //
            //
            // build ODE Step
            //
            //
            //__________________________________________________________________
            {
                Y_XML_SECTION(xml, "ODE");
                computeRate(dC);
                xreal_t scale;
                if( stepWasCut(Cend,Cini,dC, &scale) )
                {
                    Y_XML_COMMENT(xml, "scaled by " << real_t(scale) );
                }
                else
                {
                    Y_XML_COMMENT(xml, "full step");
                }
                saveProfile("ode.pro",1000);

                const xreal_t slope = aftermath.xadd.dot(gradient,dC);
                if(slope.mantissa<0.0)
                {
                    Y_XML_COMMENT(xml,"negative ODE slope");
                    const xreal_t    ode = lookUp();
                    Y_XMLOG(xml, "|ode=" << Formatted::Get("%15.4g",real_t(ode)) << "|");
                    if(ode<ff1)
                    {
                        Y_XML_COMMENT(xml,"upgrade ODE result");
                        ff1 = ode;
                        Copt.ld(Ctmp);
                        if(ff1.mantissa<=0)
                        {
                            Y_XML_COMMENT(xml, "solving by ODE step");
                            mine.transfer(C,L,Copt,SubLevel);
                            Y_DEVICE_RETURN(Solved);
                        }
                    }
                    else
                    {
                        Y_XML_COMMENT(xml,"discard ODE result");
                    }
                }
                else
                {
                    Y_XML_COMMENT(xml,"positive ODE slope");
                }

            }
            assert(ff1.mantissa>0.0);

            //__________________________________________________________________
            //
            //
            //
            // build NRA Step
            //
            //
            //__________________________________________________________________
            {
                Y_XML_SECTION(xml, "NRA");
                const size_t n = basis.size;
                const size_t m = nspc;
                //--------------------------------------------------------------
                //
                //
                // initialize Phi and Nu
                //
                //
                //--------------------------------------------------------------
                XMatrix &Phi = xPhi[n]; assert(Phi.rows  == n);
                XMatrix &Nu  = xNu[n];  assert(Nu.rows   == n);
                XMatrix &Chi = xChi[n]; assert(Chi.rows  == n);
                XArray  &xi  = xXi[n];  assert(xi.size() == n);

                {
                    const ANode *an = basis.head;
                    for(size_t i=1;i<=n;++i,an=an->next)
                    {
                        assert(0!=an);
                        const Ansatz      &ans = **an;
                        const Equilibrium &eq  = ans.eq;
                        XWritable         &phi = Phi[i];
                        XWritable         &nu  = Nu[i];
                        eq.topology(nu, SubLevel);
                        eq.drvsAffinity(phi, SubLevel, Cini, SubLevel);
                        (xi[i] = eq.affinity(ans.ek, aftermath.xmul, Cini, SubLevel)).neg();
                    }
                }

                //--------------------------------------------------------------
                //
                //
                // compute Phi*Nu'
                //
                //
                //--------------------------------------------------------------
                XAdd   &xadd = aftermath.xadd;
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        xadd.free();
                        for(size_t k=m;k>0;--k)
                        {
                            xadd << Phi[i][k] * Nu[j][k];
                        }
                        Chi[i][j] = xadd.sum();
                    }
                }


                //--------------------------------------------------------------
                //
                //
                // compute inv(Phi*Nu')
                //
                //
                //--------------------------------------------------------------
                if(xlu.build(Chi))
                {
                    //----------------------------------------------------------
                    //
                    // compute NRA dC
                    //
                    //----------------------------------------------------------
                    xlu.solve(Chi,xi);
                    for(size_t j=m;j>0;--j)
                    {
                        xadd.free();
                        for(size_t k=n;k>0;--k)
                        {
                            xadd << Nu[k][j] * xi[k];
                        }
                        dC[j] = xadd.sum();
                    }

                    //----------------------------------------------------------
                    //
                    // cut
                    //
                    //----------------------------------------------------------
                    xreal_t scale;
                    if( stepWasCut(Cend,Cini,dC, &scale) )
                    {
                        Y_XML_COMMENT(xml, "scaled by " << real_t(scale) );
                    }
                    else
                    {
                        Y_XML_COMMENT(xml, "full step");
                    }
                    saveProfile("nra.pro",1000);

                    const xreal_t slope = aftermath.xadd.dot(gradient,dC);
                    if(slope.mantissa<0.0)
                    {
                        Y_XML_COMMENT(xml,"negative NRA slope");
                        const xreal_t    nra = lookUp();
                        Y_XMLOG(xml, "|nra=" << Formatted::Get("%15.4g",real_t(nra)) << "|");
                        if(nra<ff1)
                        {
                            Y_XML_COMMENT(xml,"upgrade NRA result");
                            ff1 = nra;
                            Copt.ld(Ctmp);
                            if(ff1.mantissa<=0)
                            {
                                Y_XML_COMMENT(xml,"solving by NRA step");
                                mine.transfer(C,L,Copt,SubLevel);
                                Y_DEVICE_RETURN(Solved);
                            }
                        }
                        else
                        {
                            Y_XML_COMMENT(xml,"discard NRA result");
                        }
                    }
                    else
                    {
                        Y_XML_COMMENT(xml,"positive NRA slope");
                    }

                }
                else
                {
                    Y_XML_COMMENT(xml, "singular matrix");
                }
            }


            //__________________________________________________________________
            //
            //
            //
            // update and return status
            //
            //
            //__________________________________________________________________
            assert(ff1<=ff0);
            assert(ff1.mantissa>0.0);
            Y_XML_COMMENT(xml,"improved solution");
            mine.transfer(C,L,Copt,SubLevel);
            Y_DEVICE_RETURN(Better);
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
            // initialize increases
            //
            //------------------------------------------------------------------
            increase.forEach( &XAdd::free );

            //------------------------------------------------------------------
            //
            // use Ansatz to compute increases
            //
            //------------------------------------------------------------------
            for(size_t i=ansatz.size();i>0;--i)
            {
                const Ansatz &ans = ansatz[i];
                if(!ans.ok) continue; // shortcut for zero
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





        bool Device:: stepWasCut(XWritable &       target,
                                 const XReadable & source,
                                 XWritable &       deltaC,
                                 xreal_t * const   result) const
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            xreal_t scale = 1.0;
            bool    abate = false;
            assert( basisOkWith(source,SubLevel) );
            assert(target.size()==source.size());
            assert(target.size()==deltaC.size());

            //------------------------------------------------------------------
            //
            //
            // loop over components, act on negative deltaC
            //
            //
            //------------------------------------------------------------------
            const size_t m = target.size();
            for(size_t j=m;j>0;--j)
            {
                const xreal_t d = deltaC[j];  if(d.mantissa>=0) continue;
                const xreal_t c = source[j];  assert(c.mantissa>=0);
                const xreal_t f = c/(-d);
                if(f<=scale)
                {
                    abate = true;
                    scale = f;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // apply safety
            //
            //
            //------------------------------------------------------------------
            if( abate )
                scale *= shield;

            //------------------------------------------------------------------
            //
            //
            // generate
            //
            //
            //------------------------------------------------------------------
        GENERATE:
            for(size_t j=m;j>0;--j)
                target[j] = source[j] + scale * deltaC[j];

            if( !basisOkWith(target,SubLevel) )
            {
                scale *= shield;
                abate  = true;
                goto GENERATE;
            }

            if(result) *result = scale;

            //------------------------------------------------------------------
            //
            //
            // recompute effective step
            //
            //
            //------------------------------------------------------------------
            for(size_t j=m;j>0;--j)
            {
                deltaC[j] = target[j] - source[j];
            }


            return abate;
        }


        void Device:: saveProfile(const String &fileName, const size_t np)
        {
            OutputFile fp(fileName);
            for(size_t i=0;i<=np;++i)
            {
                const double u = double(i)/np;
                const double f = double( (*this)(u) );
                fp("%.15g %.15g\n", u, f);
            }
        }


    }

}

