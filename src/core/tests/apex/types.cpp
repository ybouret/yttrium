#include "y/apex/block/factory.hpp"

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

    {
        Block::List b;
        for(size_t iter=0;iter<20;++iter)
        {
            b.pushTail( F.acquire( ran.in(0,10) ) );
        }
        while(b.size) F.release(b.popTail());
    }

    F.display();
    F.gc();
    F.display();
    F.gc();
    F.display();

    Apex::Block b(0);
#if 0
    const size_t maxBits = b.range * 8;
    std::cerr << "maxBits=" << maxBits << std::endl;
    for(size_t i=0;i<=maxBits;++i)
    {
        std::cerr << "bits=" << std::setw(4) << i;
        for(size_t j=0;j<Apex::JigAPI::Plans;++j)
        {
            Apex::JigAPI &jig = b[ Apex::Plan(j) ];
            jig.updateFor(i);
            std::cerr << " | " << jig.words;
        }

        std::cerr << std::endl;
    }
#endif

    std::cerr << "Plan1" << std::endl;
    b.as<Plan1>().word[0] = 0x12;
    b.as<Plan1>().word[1] = 0x0a;
    b.as<Plan1>().word[2] = 0xe0;
    b.sync();
    std::cerr << "b="    << b      << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;


    std::cerr << "Plan2" << std::endl;
    b.to(Plan2);
    std::cerr << "b=" << b << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;

    std::cerr << "Plan1" << std::endl;
    b.to(Plan1);
    std::cerr << "b=" << b << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;



    for(unsigned nbit=0;nbit<=10;++nbit)
    {
        Apex::Block * p = F.acquire(ran,nbit);
        std::cerr << *p << std::endl;
        F.release(p);
    }
}
Y_UDONE()


