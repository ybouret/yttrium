
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

using namespace Yttrium;



Y_UTEST(ordered_heap)
{

    Random::Rand ran;
    

    Heap< apn, Core::FlexibleRawBuffer<apn,Memory::Dyadic> > h;
    Heap< apn, Core::CompiledRawBuffer<13,apn>  >            hf;



    for(int i=0;i<10;++i)
    {
        const apn n(1+ran.leq(30),ran);
        h.insert(n);
        hf.insert(n);
        std::cerr << h << "/" << hf << std::endl;
    }

    while(h.size())
    {
        const apn top = h.pull();
        std::cerr << std::setw(20) << top << " <- " << h << std::endl;
    }


    std::cerr << "PrivateBytes="  << hf.PrivateBytes    << std::endl;
    std::cerr << "WorkspaceSize=" << hf.WorkspaceSize() << std::endl;



}
Y_UDONE()


