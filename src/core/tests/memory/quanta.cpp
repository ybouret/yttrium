
#include "y/memory/quanta.hpp"
#include "y/memory/straps.hpp"
#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/data/list/cxx.hpp"
#include "y/random/shuffle.hpp"
#include "y/memory/out-of-reach.hpp"

using namespace Yttrium;


Y_UTEST(memory_quanta)
{

    Y_SIZEOF(Memory::Blocks);
    Y_SIZEOF(Memory::Straps);

    std::cerr << "#blocks = " << sizeof(Memory::Blocks) / sizeof(void*) << std::endl;
    std::cerr << "#straps = " << sizeof(Memory::Straps) / sizeof(void*) << std::endl;


}
Y_UDONE()


