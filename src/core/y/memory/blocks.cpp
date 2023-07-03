#include "y/memory/blocks.hpp"
#include "y/memory/album.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/memory/out-of-reach.hpp"

#include <cerrno>

namespace Yttrium
{

    namespace Memory
    {

        const char * const Blocks:: CallSign = "Memory::Blocks";

        Blocks:: ~Blocks() noexcept
        {
            while(numArenas>0)
            {
                OutOfReach::Naught(&arenaHead[--numArenas]);
            }
            dataPages->store(arenaHead);
            arenaHead = 0;
            dataPages = 0;
            arenaTail = 0;
            available = 0;
        }

        static inline
        unsigned InitialShiftFor(size_t &total)
        {
            static const size_t bytesPerArena = sizeof(Arena);
            const size_t        requiredBytes = bytesPerArena * total;
            if(requiredBytes>=Base2<size_t>::MaxPowerOfTwo)
                throw Specific::Exception(Blocks::CallSign,"initial total is too big");

            unsigned shift = 0;
            size_t   bytes = 1;
            while(bytes<requiredBytes)
            {
                ++shift;
                bytes <<= 1;
            }
            assert( (Base2<size_t>::One << shift) == bytes);

            while(shift<Pages::MinShift)
            {
                ++shift;
                bytes <<= 1;
            }
            assert( (Base2<size_t>::One << shift) == bytes);
            total = bytes/bytesPerArena;
            return shift;
        }

        Blocks:: Blocks(Album &userAlbum) :
        album(userAlbum),
        numArenas( 0 ),
        available( Max(MinAvailable,Page::DefaultBytes/sizeof(Arena)) ),
        acquiring(0),
        releasing(0),
        dataPages( &album[InitialShiftFor(available)] ),
        arenaHead( static_cast<Arena *>( dataPages->query() ) ),
        arenaTail( arenaHead-1 )
        {
            
        }

        static inline unsigned CheckShift(const unsigned newShift)
        {
            if(newShift>=Base2<size_t>::MaxShift)
                throw Specific::Exception(Blocks::CallSign,"capacity exceeded in expand()");
            return newShift;
        }

        void Blocks:: expand()
        {
            assert(numArenas<=available);
            const unsigned  newShift     = CheckShift(dataPages->shift + 1);
            Pages *         newDataPages = &album[newShift];
            Arena *         newArenaHead = static_cast<Arena *>(newDataPages->query()); assert(newDataPages->bytes>dataPages->bytes);
            const size_t    newAvailable = newDataPages->bytes / sizeof(Arena);     assert(newAvailable>available);

            OutOfReach::Grab(newArenaHead,arenaHead,numArenas*sizeof(Arena));
            dataPages->store(arenaHead);

            dataPages     = newDataPages;
            available     = newAvailable;
            arenaHead     = newArenaHead;
            arenaTail     = (arenaHead+numArenas)-1;
        }

        
    }

}

