
#include "y/memory/strap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(memory_strap)
{
    Y_SIZEOF(Memory::Strap);
    Y_SIZEOF(Memory::Strap::Block);

    std::cerr << "Memory::Strap::MinRequest = " << Memory::Strap:: MinRequest << std::endl;

    void          *wksp[64];
    Memory::Strap *strap = new (wksp) Memory::Strap(wksp,sizeof(wksp));
    std::cerr << "head@" << strap->head << ", size=" << strap->head->size << std::endl;
    std::cerr << "tail@" << strap->tail << std::endl;


    for(size_t blockSize = 0; blockSize<=40;++blockSize)
    {
        std::cerr << blockSize << " -> " << Memory::Strap::BlockSize(blockSize) << std::endl;
    }



}
Y_UDONE()

