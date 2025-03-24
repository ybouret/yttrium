#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/boolean.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        using namespace MKL;

        Reactor:: Reactor(const Cluster &persistentCluster) :
        cluster(persistentCluster),
        solve1D(),
        x_score(),
        running(),
        basis(),
        Ceq(cluster->equilibria->size,cluster->species->size),
        Cini(cluster->species->size),
        Cend(cluster->species->size),
        Ctry(cluster->species->size),
        dC(cluster->species->size),
        Cwin(cluster->species->size),
        qMetrics(cluster->species->size),
        qVCache( new QVector::Cache(qMetrics) ),
        qFamily( qVCache ),
        rate(cluster->species->size),
        tracing(4,AsCapacity)
        {
            running.proxy->reserve(Ceq.rows);
        }

        Reactor:: ~Reactor() noexcept
        {
        }

        bool Reactor:: IsRunning(const Outcome &out) noexcept
        {
            assert(Blocked!=out.st);
            return Running == out.st;
        }


        xreal_t Reactor:: score(const XReadable &C,
                                const Level      L)
        {
            assert(running.size>0);
            x_score.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const xreal_t aff = (**node).affinity(solve1D.xadd,C,L);
                x_score << Squared(aff);
            }
            const xreal_t numer = x_score.sum();
            const xreal_t denom = running.size;
            const xreal_t ratio = numer/denom;
            return ratio.sqrt();
        }


        xreal_t Reactor:: operator()(const xreal_t u)
        {
            const xreal_t v   = xreal_t(1)-u;
            for(size_t i=Ctry.size();i>0;--i)
            {
                const xreal_t cv = Cini[i];
                const xreal_t cu = Cend[i];
                xreal_t       cmin = cv, cmax=cu;
                if(cmax<cmin)
                    Swap(cmin,cmax);
                assert(cmin<=cmax);
                Ctry[i] = Clamp(cmin,cv*v+cu*u,cmax);
            }

            return score(Ctry,SubLevel);
        }

        SignType Reactor:: ByDecreasingAX(const OutNode * const lhs,
                                          const OutNode * const rhs) noexcept
        {
            return Sign::Of( (**rhs).ax, (**lhs).ax );
        }

        SignType Reactor:: ByIncreasingSC(const OutNode * const lhs,
                                          const OutNode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).sc, (**rhs).sc );
        }





        void Reactor:: finalize(XMLog     &xml,
                                XWritable &C0,
                                const bool exact)
        {
            Y_XML_SECTION_OPT(xml,"Finalize", "exact=" << BooleanTo::text(exact));
            cluster.expand(C0,Cwin);
            if(xml.verbose)
            {
                cluster.show(xml(), TopLevel, "\t[", C0, "]", xreal_t::ToString);
            }
        }


#define Y_ChemicalReactor(CALL,UUID) do {\
/**/ const xreal_t Stmp = CALL; if( converged(xml,Stmp,UUID,Swin,Mwin) ) return finalize(xml,C0,true);\
} while(false)


        void Reactor:: operator()(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            static const char fn[] = "reactor.dat";
            Y_XML_SECTION(xml, "Reactor");

            if(Trace)
            {
                OutputFile::Overwrite(fn);
            }

            unsigned cycle = 0;
        CYCLE:
            {
                Y_XML_COMMENT(xml, "*** BestEffort#" << cycle+1 << " ***");

                if(Trace) eraseOlderProfiles();
                
                const xreal_t S0   = getRunning(xml,C0,K0); if(running.size<=0) { Y_XML_COMMENT(xml, "All Blocked"); return; }
                xreal_t       Swin = S0; Cwin.ld(Cini);
                const char *  Mwin = GetRunning;

                if(Trace && cycle<=0) {
                    AppendFile fp(fn); fp("%u ", cycle) << Swin.str() << " #" << Mwin << "\n";
                }
                ++cycle;



                Y_ChemicalReactor(narrowDown(xml,S0),    NarrowDown);
                Y_ChemicalReactor(queryRates(xml,S0),    QueryRates);
                Y_ChemicalReactor(generateNR(xml,S0,K0), GenerateNR);


                if(Trace)
                {
                    emitGnuPlotTracing(std::cerr);
                    AppendFile fp(fn); fp("%u ", cycle) << Swin.str() << " #" << Mwin << "\n";
                }

                if(Swin>=S0)
                {
                    return finalize(xml,C0,false);
                }

                cluster.expand(C0,Cwin);
                if(cycle<8) goto CYCLE;
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
