

#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"

#include "y/utest/run.hpp"
#include "../alea.hpp"


using namespace Yttrium;


Y_UTEST(memory_arena)
{
    alea_seed();

    Y_SIZEOF(Memory::Arena);

    size_t pageBytes = 4096;

    for(size_t blockSize = 1; blockSize <= 256; ++blockSize )
    {
        uint8_t        nblks = 0;
        const unsigned shift = Memory::Arena::ComputeShift(blockSize, pageBytes, nblks);
        const size_t   bytes = Base2<size_t>::One << shift;
        const size_t   loss  = bytes - (sizeof(Memory::Chunk) + nblks * blockSize);
        std::cerr << "blockSize = " << std::setw(4) << blockSize
        << " : pageSize = " << std::setw(6) << pageBytes
        << " -> "           << std::setw(6) << bytes << " : #" << std::setw(4) << int(nblks) << " | loss=" << loss << std::endl;

    }



}
Y_UDONE()
