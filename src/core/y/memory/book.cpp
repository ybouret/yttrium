
#include "y/memory/book.hpp"
#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {

        Book:: ~Book() noexcept
        {

        }

        Book:: Book(Album &userAlbum) :
        album(userAlbum),
        notes(album)
        {
            
        }

        Dyad & Book:: operator[](const unsigned shift) noexcept
        {
            assert(shift<=MaxShift);
            if (shift<MidShift) return notes[shift]; else return album[shift];
        }

    }

}

