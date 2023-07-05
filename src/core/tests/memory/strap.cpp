
#include "y/memory/strap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(memory_strap)
{


    void          *wksp[64];
    Memory::Strap *strap = new (wksp) Memory::Strap(wksp,sizeof(wksp));

    size_t blockSize = 0;
    void  *blockAddr = strap->acquire(blockSize);
    std::cerr << blockSize << " @" << blockAddr << std::endl;

    strap->displayInfo(0);



}
Y_UDONE()

