#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Reactor:: narrowDown(XMLog &xml, const xreal_t S0)
        {
            static const char fn[] = "NarrowDown";
            Y_XML_SECTION_OPT(xml,fn, "running=" << running.size);


            //------------------------------------------------------------------
            //
            //
            // optimize each 1D
            //
            //
            //------------------------------------------------------------------
            xreal_t Sn = S0;
            for(OutNode *node=running.head;node;node=node->next)
            {
                Outcome &out = **node;

                //--------------------------------------------------------------
                //
                // Cend = cc
                //
                //--------------------------------------------------------------
                cluster.transfer(Cend,SubLevel,out.cc, out.lv);

                if(Trace) saveCurrentProfile(*out.eq.name, 100);


                //--------------------------------------------------------------
                //
                // global optimize on Cini:Cend
                //
                //--------------------------------------------------------------
                const  xreal_t Stry = optimize1D(S0);

                if(Stry<S0)
                {
                    //----------------------------------------------------------
                    // better: update
                    //----------------------------------------------------------
                    cluster.transfer(out.cc,out.lv,Ctry,SubLevel);
                    out.xi = out.eq.extent(x_score, out.cc, out.lv, Cini, SubLevel);
                    out.ax = out.xi.abs();
                    out.sc = Stry;
                    Y_XMLOG(xml,"[*] " << out);
                    InSituMin(Sn,Stry);
                }
                else
                {
                    //--------------------------------------------------------------
                    // not better: pinned down
                    //--------------------------------------------------------------
                    cluster.transfer(out.cc,out.lv,Cini,SubLevel);
                    out.xi = 0;
                    out.ax = 0;
                    out.sc = S0;
                    Y_XMLOG(xml,"[0] " << out);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // build local basis
            //
            //
            //------------------------------------------------------------------
            MergeSort::Call(running,ByIncreasingSC);
            const size_t dof = cluster.N;
            qFamily.clear();
            basis.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Equilibrium   &eq = (**node).eq;
                const Readable<int> &nu = eq(cluster.iNu,SubLevel);
                if(qFamily.welcomes(nu))
                {
                    (void) qFamily.increase();
                    basis << eq;
                    if(qFamily->size>=dof) break;
                }
            }

            if(basis.size) tracing << *(**basis.head).name;

            if(xml.verbose)
            {
                Y_XML_COMMENT(xml, "Basis and Dependents");
                for(const OutNode *node=running.head;node;node=node->next)
                {
                    const Equilibrium &eq = (**node).eq;
                    if(basis.has(eq)) {
                        Y_XMLOG(xml, "(*) " << **node);
                    }
                    else {
                        Y_XMLOG(xml, "    " << **node);
                    }
                }
            }

            Y_XML_COMMENT(xml, fn << " result");
            Y_XMLOG(xml, "Sx = " << Sn.str() << " // S0=" << S0.str() );
            return Sn;
        }
    }

}
