

#include "y/ordered/core/compiled-raw-buffer.hpp"
#include "y/ordered/core/flexible-raw-buffer.hpp"
#include "y/ordered/dire.hpp"

#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/kemp/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/system/exception.hpp"

using namespace Yttrium;



Y_UTEST(ordered_dire)
{

    Random::Rand ran;



    Dire< apn, Core::FlexibleRawBuffer<apn,Memory::Dyadic> > Dd;
    Dire< apn, Core::CompiledRawBuffer<13,apn>  >            Df;

#if 1
    for(int i=0;i<10;++i)
    {
        const apn n(1+ran.leq(30),ran);
        Dd.insert(n);
        Df.insert(n);
        std::cerr << Dd << "/" << Df << std::endl;
    }

    while(Dd.size())
    {
        const apn top = Dd.pull();
        std::cerr << std::setw(20) << top << " <- " << Dd << std::endl;
    }
#endif


}
Y_UDONE()


