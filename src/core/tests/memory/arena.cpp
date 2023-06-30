

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

    for(size_t blockSize = 1; blockSize <= 10; ++blockSize )
    {
        const unsigned shift = Memory::Arena::ComputeShift(blockSize, pageBytes);
        std::cerr << "blockSize = " << blockSize << " => " << (Base2<size_t>::One << shift) << std::endl;
    }



}
Y_UDONE()
