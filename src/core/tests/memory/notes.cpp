#include "y/memory/album.hpp"
#include "y/memory/notes.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"

#include <cstring>

using namespace Yttrium;

struct block_t
{
    void  *addr;
    size_t size;
};

Y_UTEST(memory_notes)
{
    alea_seed();

    Memory::Album  album;
    {
        Memory::Notes notes(album);
        for(unsigned shift=0;shift<=Memory::Notes::MaxShift;++shift)
        {
            Memory::Dyad &dyad = notes[shift];
            dyad.release( dyad.acquire() );
        }
    }
    album.displayInfo(0);



}
Y_UDONE()
