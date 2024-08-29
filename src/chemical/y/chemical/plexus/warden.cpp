
#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Warden:: ~Warden() noexcept {}


        Warden:: Warden(const Cluster &cluster) :
        mine(cluster),
        head( mine.laws.isValid() ? mine.laws->groups.head : 0),
        rows( (0!=head) ? mine.laws->maxGroupSize : 0 ),
        cols( (0!=head) ? mine.species.size       : 0 ),
        xadd( cols ),
        conc( rows, cols),
        jail( rows ),
        cinj( cols ),
        fund(),
        lawz(fund.lbank),
        trms(fund),
        best(fund.sbank),
        wobbly(fund.sbank),
        ctrade(mine.size,mine.species.size),
        dtrade(mine.size,mine.species.size),
        trades(mine.size),
        xaccum(mine.species.size)
        {
        }

        void Warden:: prolog() noexcept
        {
            cinj.forEach( & XAdd::free );
        }

    }


}
