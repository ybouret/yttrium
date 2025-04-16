
#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/boolean.hpp"
#include "y/text/plural.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        bool Reactor:: MonitorScore = false;

        void Reactor:: finalize(XMLog     &xml,
                                XWritable &C0,
                                const bool exact)
        {
            Y_XML_SECTION_OPT(xml,"Finalize", "exact=" << BooleanTo::text(exact));

            // transfer solution
            cluster.expand(C0,Cwin);

            // summary
            if(xml.verbose)
            {

                Y_XML_COMMENT(xml, "current concentration" << Plural::s(cluster->species->size) );
                for(const SNode *sn = cluster->species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    cluster->sformat.pad( xml() << "[" << sp.name << "]", sp) << " = " << sp(C0,TopLevel).str() << std::endl;
                }

                Y_XML_COMMENT(xml, "running equilibri" << Plural::aum(running.size) );
                for(const OutNode *node=running.head;node;node=node->next)
                {
                    const Outcome &out = **node;
                    cluster->display(xml(), out.eq)  << std::setw(24) << out.activity(solve1D.xmul, C0, TopLevel).str() <<  std::endl;
                }
            }
        }


#define Y_ChemicalReactor(CALL,UUID) do {\
/**/ const xreal_t Stmp = CALL; if( converged(xml,Stmp,UUID,Swin,Mwin) ) return finalize(xml,C0,true);\
} while(false)


        void Reactor:: mix(XMLog &             xml,
                           XWritable &         C0,
                           const XReadable &   K0,
                           Proc *        const cb)
        {
            static const char fn[] = "reactor.dat";
            Y_XML_SECTION(xml, "Reactor");


            if(MonitorScore) OutputFile::Overwrite(fn);

            unsigned cycle = 0;
        CYCLE:
            {
                Y_XML_COMMENT(xml, " ---- Cycle#" << cycle+1 << " ----");

                if(EmitProfiles) eraseOlderProfiles();

                const xreal_t S0   = getRunning(xml,C0,K0); if(running.size<=0) { Y_XML_COMMENT(xml, "All Blocked"); return; }
                xreal_t       Swin = S0; Cwin.ld(Cini);
                const char *  Mwin = GetRunning;

                if(MonitorScore && cycle<=0) {
                    AppendFile fp(fn); fp("%u ", cycle) << Swin.str() << " #" << Mwin << "\n";
                }
                ++cycle;



                Y_ChemicalReactor(narrowDown(xml,S0),    NarrowDown);
                Y_ChemicalReactor(queryRates(xml,S0),    QueryRates);
                Y_ChemicalReactor(generateNR(xml,S0,K0), GenerateNR);



                if(EmitProfiles) emitGnuPlotTracing(std::cerr);
                
                if(MonitorScore) {
                    AppendFile fp(fn); fp("%u ", cycle) << Swin.str() << " #" << Mwin << "\n";
                }

                if(Swin>=S0) return finalize(xml,C0,false);


                cluster.expand(C0,Cwin);
                if(cb) (*cb)(C0,Swin);

                goto CYCLE;
            }

        }


        void Reactor:: increaseRates(const xreal_t px, const Components &eq)
        {
            // apply product extent
            for(const Actor *a=eq.prod->head;a;a=a->next) a->sp(rate,SubLevel).insert(px,a->nu);

            // apply reactant extent
            const xreal_t rx = -px;
            for(const Actor *a=eq.reac->head;a;a=a->next) a->sp(rate,SubLevel).insert(rx,a->nu);
        }

        bool Reactor:: converged(XMLog &            xml,
                                 const xreal_t      Stry,
                                 const char * const Mtry,
                                 xreal_t      &     Swin,
                                 const char * &     Mwin)
        {
            assert(0!=Mtry);
            assert(0!=Mwin);
            if(Stry<Swin)
            {
                Y_XMLOG(xml, "(+) improved " << Mtry << "@" << Stry.str() << " // " << Mwin << "@" << Swin.str());
                Swin =  Stry;
                Mwin =  Mtry;
                Cwin.ld(Ctry);
                if(Stry<=0.0)
                {
                    return true;
                }
                else
                    return false;
            }
            else
            {
                Y_XMLOG(xml, "(+) discard  " << Mtry << "@" << Stry.str() << " // " << Mwin << "@" << Swin.str());
                return false;
            }
        }
    }

}


