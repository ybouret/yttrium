

#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/album.hpp"
#include "y/utest/run.hpp"
#include "y/lockable.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


Y_UTEST(memory_quarry)
{
    Random::Rand ran;
    Y_SIZEOF(Memory::Quarry::Stone);
    Y_SIZEOF(Memory::Quarry::Vein);

    Memory::Album  album;
    Memory::Corpus corpus(album);
    Memory::Quarry quarry(corpus);

    std::cerr << "Required: " << Memory::Quarry::Required << std::endl;

}
Y_UDONE()
