
#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/ordered/heap.hpp"

#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/apex/natural.hpp"
#include "y/comparison.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/system/exception.hpp"
#include "y/type/capacity.hpp"

using namespace Yttrium;



Y_UTEST(ordered_heap)
{

    Random::Rand ran;
    {
        const int    arr[] = {1,2,3,4,5,6,7,8};
        const size_t num   = sizeof(arr)/sizeof(arr[0]);
        Random::Shuffle::Range(arr,num,ran);
        Core::Display(std::cerr,arr,num) << std::endl;

        Core::CompiledRawBuffer<8,int>              cbuf;
        Core::FlexibleRawBuffer<int,Memory::Pooled> fbuf0;
        Core::FlexibleRawBuffer<int,Memory::Dyadic> fbuf(3);



        for(size_t i=0;i<num;++i)
        {
            cbuf.insertWith(  Comparison::CxxIncreasing<double>, arr[i]  );
            fbuf0.insertWith( Comparison::CxxIncreasing<double>, arr[i]  );
            fbuf.insertWith(  Comparison::CxxIncreasing<double>, arr[i]  );
            Core::Display(std::cerr,cbuf.entry,cbuf.count) << " -> " << cbuf.peek() << std::endl;
        }

        while(cbuf.size())
        {
            const int top = cbuf.uprootWith(Comparison::CxxIncreasing<double>);
            std::cerr << top << " <- ";  Core::Display(std::cerr,cbuf.entry,cbuf.count) << std::endl;
        }


    }


    Heap< apn,Core::FlexibleRawBuffer<apn,Memory::Dyadic> > h;

    for(int i=0;i<10;++i)
    {
        const apn n(1+ran.leq(30),ran);
        h.insert(n);
        std::cerr << h << std::endl;

    }
    while(h.size())
    {
        const apn top = h.pull();
        std::cerr << std::setw(20) << top << " <- " << h << std::endl;
    }



}
Y_UDONE()


