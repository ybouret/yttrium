
#include "y/memory/corpus.hpp"
#include "y/memory/album.hpp"

namespace Yttrium
{

    namespace Memory
    {

        Corpus:: ~Corpus() noexcept
        {

        }

        Corpus:: Corpus(Album &userAlbum) :
        album(userAlbum),
        notes(album)
        {
            
        }

        Dyad & Corpus:: operator[](const unsigned shift) noexcept
        {
            assert(shift<=MaxShift);
            if (shift<MidShift) return notes[shift]; else return album[shift];
        }

        void Corpus:: displayInfo(const size_t indent) const
        {
            Core::Indent(std::cerr,indent) << "<Corpus>" << std::endl;
            notes.displayInfo(indent+2);
            album.displayInfo(indent+2);
            Core::Indent(std::cerr,indent) << "<Corpus/>" << std::endl;

        }

    }

}

