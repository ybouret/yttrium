
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
            for(size_t i=1;i<=nmax;++i)
            {
                const Applicant   &lhs = aps[i]; if(xml.verbose)  lhs.display( xml() << "| ", rcl.uuid, true) << std::endl;
                const Equilibrium &lEq = lhs.eq;
                const char * const lid = lEq.name.c_str();

                if( !lEq.reac.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing reactant(s) on top-level");
                if( !lEq.prod.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing product(s) on top-level");

                for(size_t j=1;j<=nmax;++j)
                {
                    const Applicant   &rhs = aps[j];
                    const char * const rid = rhs.eq.name.c_str();
                    if(!lEq.reac.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                    if(!lEq.prod.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                }
            }

            return nmax;
        }


    }

}

