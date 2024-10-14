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

            // at this point, we can compute objectiveFunction

            //__________________________________________________________________
            //
            //
            //
            // Sorting out Running ansatz
            //
            //
            //__________________________________________________________________
            const size_t na = ansatz.size();
#           include "running.hxx"

           


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
            // build basis IN ANY CASE for asymptotic method
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
            Y_XML_COMMENT(xml,"improved solution: " << real_t(ff0) << " => " << real_t(ff1));
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

