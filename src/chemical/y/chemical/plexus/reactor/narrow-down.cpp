#include "y/chemical/plexus/reactor.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Reactor:: NarrowDown = "NarrowDown";


        xreal_t Reactor:: narrowDown(XMLog &xml, const xreal_t S0)
        {
            Y_XML_SECTION_OPT(xml,NarrowDown, "running=" << running.size);
            assert(0==tighten.size);

            //------------------------------------------------------------------
            //
            //
            // optimize each 1D
            //
            //
            //------------------------------------------------------------------
            xreal_t          Snd = S0;
            const XReadable *ptr = 0;         // save best concentration
            Level            lvl = SubLevel;  // save best level

            for(OutNode *node=running.head;node;node=node->next)
            {
                Outcome &out = **node;

                //--------------------------------------------------------------
                //
                // load Cend with out.cc
                //
                //--------------------------------------------------------------
                cluster.transfer(Cend,SubLevel,out.cc, out.lv);

                if(EmitProfiles) saveCurrentProfile(*out.eq.name, 100);


                //--------------------------------------------------------------
                //
                // optimize on [Cini:Cend]
                //
                //--------------------------------------------------------------
                const  xreal_t Stry = optimize1D(S0);

                if(Stry<S0)
                {
                    //----------------------------------------------------------
                    // better: update with Ctry
                    //----------------------------------------------------------
                    cluster.transfer(out.cc,out.lv,Ctry,SubLevel);
                    out.xi = out.eq.extent(x_score, out.cc, out.lv, Cini, SubLevel);
                    out.ax = out.xi.abs();
                    out.sc = Stry;
                    out.wr = S0-Stry;
                    Y_XMLOG(xml,"[*] " << out);
                    if(Stry<Snd)
                    {
                        Snd =  Stry;
                        ptr = &out.cc;
                        lvl =  out.lv;
                    }
                    tighten << out;
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
                    out.wr = 0;
                    Y_XMLOG(xml,"[0] " << out);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // build local basis using most promising equilibria
            //
            //
            //------------------------------------------------------------------
            MergeSort::Call(running,ByIncreasingSC);
            const size_t dof = cluster.N;
            qFamily.clear();
            basis.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Outcome       &out = **node;
                const Equilibrium   &eq = out.eq;
                const Readable<int> &nu = eq(cluster.iNu,SubLevel);
                if(qFamily.welcomes(nu))
                {
                    (void) qFamily.increase();
                    basis << eq;
                    Coerce(eq.indx[AuxLevel]) = basis.size;
                    if(qFamily->size>=dof) break;
                }
            }

            if(basis.size>0)
            {
                const String &uid = *(**basis.head).name;
                profiles << uid;
                if(EmitProfiles) EraseIfDifferentOf(uid);
            }

            if(xml.verbose)
            {
                Y_XML_COMMENT(xml, "Basis");
                for(const OutNode *node=running.head;node;node=node->next)
                {
                    const Outcome     &out = **node;
                    const Equilibrium &eq  = out.eq;
                    const char * const mark1 = basis.has(eq) ? "#" : " ";
                    const char * const mark2 = (out.sc < S0) ? "*" : "0";
                    Y_XMLOG(xml,"(" << mark1 << mark2 << ") " << **node);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // finalize 
            //
            //
            //------------------------------------------------------------------
            if(0!=ptr) {
                assert(Snd<S0);
                cluster.transfer(Ctry,SubLevel,*ptr,lvl);
            }

            Y_XML_COMMENT(xml, NarrowDown << " result: |running|=" << running.size << " |basis|=" << basis.size << " |tighten|=" << tighten.size);
            Y_XMLOG(xml, "Snd = " << Snd.str() << " // S0=" << S0.str() );

            return Snd;
        }
    }

}
