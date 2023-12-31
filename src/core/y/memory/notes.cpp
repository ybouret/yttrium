
#include "y/memory/notes.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Memory
    {


        void Notes:: destruct(size_t built) noexcept
        {
            while(built-- > 0)
            {
                OutOfReach::Naught(&dyad[built]);
            }
            dyad = 0;
        }


        Notes:: ~Notes() noexcept
        {
            destruct(Number);
        }

        Notes:: Notes(Album &album) :
        dyad(0),
        wksp()
        {
            dyad = static_cast<DyadicArena *>(Y_STATIC_ZARR(wksp));
            unsigned built = 0;
            while(built<Number)
            {
                try {
                    new ( dyad+built ) DyadicArena(built,album);
                    ++built;
                }
                catch(...)
                {
                    destruct(built);
                    throw;
                }
            }
        }

        DyadicArena & Notes:: operator[](const size_t shift) noexcept
        {
            assert(shift>=MinShift);
            assert(shift<=MaxShift);
            assert(shift==dyad[shift].shift);
            return dyad[shift];
        }


        void Notes:: displayInfo(const size_t indent) const
        {
            Core::Indent(std::cerr,indent) << "<Memory::Notes MinShift='" << MinShift << "' MaxShift='" << MaxShift << "'>" << std::endl;
            for(unsigned shift=MinShift;shift<=MaxShift;++shift)
            {
                dyad[shift].displayInfo(indent+2);
            }
            Core::Indent(std::cerr,indent) << "<Memory::Notes/>" << std::endl;
        }


    }

}

