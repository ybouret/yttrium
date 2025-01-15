#include "y/apex/block/factory.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/concurrent/mutex/qpool.hpp"

#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"



using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_types)
{

    Random::ParkMiller ran;
    Y_SIZEOF(Apex::Block);
    Y_SIZEOF(Apex::Blocks);

    Y_SIZEOF(Jig1);
    Y_SIZEOF(Jig2);
    Y_SIZEOF(Jig4);
    Y_SIZEOF(Jig8);
    Y_SIZEOF(Jigs);


    Apex::Factory & F = Apex::Factory::Instance();






    for(unsigned nbit=0;nbit<=1000;++nbit)
    {
        Apex::BlockPtr  p = F.query(ran,nbit);
        Y_ASSERT(Plan1==p->plan);

        for(unsigned i=0;i<JigAPI::Plans;++i)
        {
            const Plan     source = Plan(i);
            const uint32_t c32    = p->to(source).crc32();
            const uint32_t t32    = p->tag32();

            // test change
            for(unsigned j=0;j<JigAPI::Plans;++j)
            {
                const Plan target = Plan(j);
                p->to(target);
                p->to(source);
                Y_ASSERT(c32==p->crc32());
                Y_ASSERT(t32==p->tag32());
            }

            // test duplication
            Apex::BlockPtr q = F.duplicate( *p );
            Y_ASSERT(q->plan==p->plan);
            Y_ASSERT(q->tag32() == p->tag32() );
        }
    }

    F.display();

    Y_SIZEOF(Mutex);

}
Y_UDONE()


