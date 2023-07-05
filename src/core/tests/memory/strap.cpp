
#include "y/memory/strap.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"

using namespace Yttrium;

Y_UTEST(memory_strap)
{

    alea_seed();
    
    void          *wksp[64];
    Memory::Strap *strap = new (wksp) Memory::Strap(wksp,sizeof(wksp));
    strap->displayInfo(0);

    while(true)
    {
        size_t blockSize = alea_leq(100);
        void  *blockAddr = strap->acquire(blockSize);
        if(!blockAddr) break;

        std::cerr << blockSize << " @" << blockAddr << std::endl;
        strap->displayInfo(0);
    }



}
Y_UDONE()

