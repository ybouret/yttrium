
#include "y/data/small/ranked.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/utest/run.hpp"
#include "y/comparison.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;

namespace {

}

Y_UTEST(data_ranked)
{

    typedef Small::BareHeavyList<int>                  iBList;
    typedef Small::Ranked<iBList,IncreasingComparator> iBRanked;
 
    typedef Small::SoloHeavyList<int>                  iSList;
    typedef Small::Ranked<iSList,IncreasingComparator> iSRanked;

    typedef Small::CoopHeavyList<int>                  iCList;
    typedef Small::Ranked<iCList,IncreasingComparator> iCRanked;
    typedef iCList::ProxyType                          Bank;


    Random::ParkMiller ran;
    Bank               bnk;

    iBRanked ibr;
    iSRanked isr;
    iCRanked icr(bnk,AsParameter);

    size_t good = 0;
    size_t drop = 0;
    for(size_t i=1;i<=10;++i)
    {
        const int value = ran.in<int>(-5,5);
        if(ibr.grow( value ))
        {
            Y_ASSERT(isr.grow(value));
            Y_ASSERT(icr.grow(value));
            ++good;
        }
        else
        {
            ++drop;
        }
    }

    std::cerr << "good=" << good << " / drop=" << drop << std::endl;
    std::cerr << ibr << std::endl;
    std::cerr << isr << std::endl;
    std::cerr << icr << std::endl;

    ibr.free();
    isr.free();


}
Y_UDONE()
