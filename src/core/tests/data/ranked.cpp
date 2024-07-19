
#include "y/data/small/ranked.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/utest/run.hpp"
#include "y/comparison.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;

namespace {

}

Y_UTEST(data_ranked)
{

    typedef Small::BareHeavyList<int>                 iList;
    typedef Small::Ranked<iList,IncreasingComparator> iRanked;

    Random::ParkMiller ran;

    iRanked ir;

    size_t good = 0;
    size_t drop = 0;
    for(size_t i=1;i<=10;++i)
    {
        if(ir.grow( ran.in<int>(-5,5) ))
        {
            ++good;
        }
        else
        {
            ++drop;
        }
    }

    std::cerr << "good=" << good << " / drop=" << drop << std::endl;
    std::cerr << ir << std::endl;

}
Y_UDONE()
