#include "y/chemical/plexus/reactor.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/boolean.hpp"
#include "y/text/plural.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {




      
        const char * const  Reactor:: CallSign = "Reactor";
        
        namespace
        {
            static inline size_t Identity(const size_t i) noexcept { return i; }
        }

        Reactor:: Reactor(const Cluster &persistentCluster) :
        cluster(persistentCluster),
        solve1D(),
        x_score(),
        running(),
        tighten(),
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
        lu(cluster.N),
        xiArr(cluster.N,Procedural,Identity),
        phiArr(cluster.N),
        jacArr(cluster.N),
        profiles(4,AsCapacity),
        next(0),
        prev(0)
        {
            // allocating resources
            running.proxy->reserve(Ceq.rows);
            qVCache->reserve(Ceq.rows);
            for(size_t n=1;n<=cluster.N;++n)
            {
                assert( n == xiArr[n].size() );
                phiArr[n].make(n,cluster.M);
                jacArr[n].make(n,n);
            }

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
            if(running.size<=0) return 0;
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





       

      

    }

}
