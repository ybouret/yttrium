
#include "y/apex/m/archon.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


Y_UTEST(apex_archon)
{
    Apex::Archon &mgr = Apex::Archon::Instance();
    std::cerr << mgr.callSign() << std::endl;

    unsigned shift = 0;
    void    *entry = mgr.acquire(shift);
    std::cerr << "got " << (1<<shift) << " bytes @" << entry << std::endl;

    mgr.release(entry,shift);

#if 0
    Apex::Archon::Block<double> b(3);
    std::cerr << "words = " << b.words << std::endl;
    std::cerr << "bytes = " << b.bytes << std::endl;
    std::cerr << "shift = " << b.shift << std::endl;
#endif
    

}
Y_UDONE()
