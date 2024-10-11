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
                Y_XML_COMMENT(xml,"Running");

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
                    Y_XML_SECTION(xml, "InitialConditions");
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
            // build NRA Step, fastest convergence if close to solution
            //
            //
            //__________________________________________________________________
#           include "nra-stp.hxx"

            //__________________________________________________________________
            //
            //
            //
            // build ODE Step, backup solution
            //
            //
            //__________________________________________________________________
#           include "ode-stp.hxx"

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

