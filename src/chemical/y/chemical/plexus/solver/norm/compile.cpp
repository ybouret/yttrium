
#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"
#include "y/sort/heap.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        size_t Normalizer:: compile(XWritable       & Ctop,
                                    const XReadable & Ktop,
                                    bool            & repl,
                                    XMLog           &  xml)
        {
            Y_XML_SECTION(xml, "Compile");

            //------------------------------------------------------------------
            //
            //
            // examine all equilibria
            //
            //
            //------------------------------------------------------------------
            repl        = false;
            size_t iter = 0;
        EXAMINE:
            ++iter;
            Y_XMLOG(xml, "[Examine] [#iter=" << iter << "]");
            aps.free();
            for(const ENode *en=rcl.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                const size_t       ii = aps.size()+1;
                XWritable         &cc = rcl.transfer(ceq[ii], SubLevel, Ctop, TopLevel);
                const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                switch(st)
                {
                    case Blocked:
                        Y_XMLOG(xml, "[Blocked] " << eq.name);
                        continue;

                    case Crucial:
                        Y_XMLOG(xml, "[Crucial] " << eq.name);
                        rcl.transfer(Ctop, TopLevel, cc, SubLevel);
                        repl = true;
                        goto EXAMINE;

                    case Running:
                        Y_XMLOG(xml, "[Running] " << eq.name);
                        break;
                }

                assert(Running==st);
                const Applicant app(eq,eK,cc, afm.eval(cc, SubLevel, Ctop, TopLevel, eq) );
                aps << app;
            }

            //------------------------------------------------------------------
            //
            //
            // sorting and cross-checking
            //
            //
            //------------------------------------------------------------------
            HeapSort::Call(aps, Applicant::Compare);
            const size_t nmax = aps.size();
            for(size_t i=nmax;i>0;--i)
            {
                const Applicant   &app = aps[i]; if(xml.verbose)  app.display( xml() << "| ", rcl.uuid, true) << std::endl;
                const Equilibrium &eq  = app.eq;
                eq.mustSupport(Ctop,TopLevel);
                for(size_t j=nmax;j>0;--j)
                    eq.mustSupport(aps[j].cc,SubLevel);
            }

            return nmax;
        }


    }

}

