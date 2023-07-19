

#include "y/ordered/prio-q.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/apex/natural.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;


Y_UTEST(ordered_prioQ)
{

    Random::Rand ran;
    static const size_t n = 20;
    static const size_t wlen = n * sizeof(apn);
    void               *wksp[ Y_WORDS_GEQ(wlen) ]; Y_ASSERT(sizeof(wksp)>=wlen);
    size_t count = 0;
    apn   *pq    = Memory::OutOfReach::Cast<apn>(wksp);
    try {
        while(count<n)
        {
            const apn temp(ran.leq(64),ran);
            std::cerr << "+" << temp << std::endl;

            Core::PrioQ<apn>::Insert(pq,count,temp,Comparison::Increasing<apn>);

        }
    }
    catch(...)
    {
        Core::PrioQ<apn>::Finish(pq,count);
        throw;
    }

    Core::PrioQ<apn>::Finish(pq,count);

}
Y_UDONE()
