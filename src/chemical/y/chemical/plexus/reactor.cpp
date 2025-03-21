#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"

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
        qMetrics(cluster->species->size),
        qVCache( new QVector::Cache(qMetrics) ),
        qFamily( qVCache ),
        rate(cluster->species->size)
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



     

        

        void Reactor:: operator()(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            Y_XML_SECTION(xml, "Reactor");
            if(Trace) eraseOlderProfiles();
            const xreal_t S0 = getRunning(xml,C0,K0); if(running.size<=0) { Y_XML_COMMENT(xml, "All Blocked"); return; }
            const xreal_t Sn = narrowDown(xml,S0);
            const xreal_t Sr = queryRates(xml,S0);

            if(Trace) emitGnuPlotTracing(std::cerr);


        }



      

    }

}
