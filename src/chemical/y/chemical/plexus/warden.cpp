
#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Warden:: CallSign = "Chemical::Warden";

        Warden:: ~Warden() noexcept {}


        Warden:: Warden(const Cluster &cluster) :
        Joint(cluster),
        head( mine.laws.isValid() ? mine.laws->groups.head : 0),
        rows( (0!=head) ? mine.laws->maxGroupSize : 0 ),
        cols( (0!=head) ? nspc                    : 0 ),
        xadd( cols ),
        conc( rows, cols),
        jail( rows ),
        cinj( nspc ),
        fund(),
        lawz(fund.lbank),
        trms(fund),
        best(fund.sbank),
        wobbly(fund.sbank),
        ctrade(neqs,nspc),
        dtrade(neqs,nspc),
        trades(neqs),
        xaccum(nspc)
        {
        }

        void Warden:: prolog() noexcept
        {
            cinj.forEach( & XAdd::free );
        }

        void Warden:: epilog(XWritable &dC, const Level L)
        {
            for(const SNode *sn=mine.species.head;sn;sn=sn->next)
            {
                const Species &      sp = **sn;
                const size_t * const id = sp.indx;
                dC[ id[L] ] = cinj[ id[SubLevel] ].sum();
            }
        }


        void Warden:: operator()(XWritable      &C,
                                 XWritable      &dC,
                                 const Level     L,
                                 XMLog          &xml)
        {
            prolog();
            sanitize(C, L, xml);
            epilog(dC,L);
        }


    }


}
