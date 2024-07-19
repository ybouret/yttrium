
#include "y/data/small/ranked.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/utest/run.hpp"
#include "y/comparison.hpp"
#include "y/random/park-miller.hpp"

#include "y/sequence/vector.hpp"

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


    Vector<int> good;
    size_t      drop=0;

    for(size_t i=1;i<=100;++i)
    {
        const int value = ran.in<int>(-30,30);
        if(ibr.insert( value ))
        {
            Y_ASSERT(isr.insert(value));
            Y_ASSERT(icr.insert(value));
            good << value;
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

    for(size_t i=1;i<=good.size();++i)
    {
        Y_ASSERT(ibr.has(good[i]));
        Y_ASSERT(isr.has(good[i]));
        Y_ASSERT(icr.has(good[i]));
    }

    while(good.size())
    {
        const int k = good.pullTail();
        Y_ASSERT(ibr.remove(k));
        Y_ASSERT(isr.remove(k));
        Y_ASSERT(icr.remove(k));

    }


    ibr.free();
    isr.free();
    icr.free();

}
Y_UDONE()
