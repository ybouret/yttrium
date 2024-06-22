
#include "y/container/cxx/series.hpp"
#include "y/utest/run.hpp"
#include "y/kemp/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/random/shuffle.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;

Y_UTEST(container_cxx_series)
{

    Random::Rand ran;

    for(size_t loop=0;loop<10;++loop)
    {
        CxxSeries<apn,Memory::Pooled> arr(5+ran.leq(20));
        for(size_t iter=0;iter<10;++iter)
        {
            arr.free();
            while(arr.size()<arr.capacity())
            {
                const apn tmp(10,ran);
                if(ran.choice())
                {
                    arr.pushHead(tmp);
                }
                else
                {
                    arr.pushTail(tmp);
                }
                std::cerr << arr << std::endl;
            }

            Iterating::DisplayAll::Of(arr);

            CxxSeries<apn,Memory::Pooled> dup(CopyOf,arr);
            Iterating::DisplayAll::Of(dup);


            while(arr.size())
            {
                if( ran.choice() )
                {
                    arr.popHead();
                }
                else
                {
                    arr.popTail();
                }
                std::cerr << arr << std::endl;
            }
        }
    }


    


}
Y_UDONE()

