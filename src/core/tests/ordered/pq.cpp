

#include "y/ordered/priority-queue.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/apex/natural.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;


Y_UTEST(ordered_PQ)
{

    Random::Rand ran;
    static const size_t n = 20;
    static const size_t wlen = n * sizeof(apn);
    void               *wksp[ Y_WORDS_GEQ(wlen) ]; Y_ASSERT(sizeof(wksp)>=wlen);
    size_t count = 0;
    apn   *pq    = Memory::OutOfReach::Cast<apn>(wksp);
    std::cerr << "Insertion" << std::endl;
    try {
        while(count<n)
        {
            const apn temp(ran.leq(16),ran);

            PriorityQueue<apn>::Insert(pq,count,temp,Comparison::Increasing<apn>);
            std::cerr << "(" << pq[0] << ") "; Core::Display(std::cerr,pq,count) << std::endl;
        }
    }
    catch(...)
    {
        PriorityQueue<apn>::Finish(pq,count);
        throw;
    }
    std::cerr << "Insertion" << std::endl;
    std::cerr << "Removal"   << std::endl;
    while(count>0)
    {
        std::cerr << "(" << pq[0] << ") " << std::endl;
        PriorityQueue<apn>::Remove(pq,count,Comparison::Increasing<apn>);
    }



}
Y_UDONE()
