
#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Memory
    {
        Quarry:: Vein:: ~Vein() noexcept
        {
            std::cerr << "-Vein @2^" << dyad.shift << " = " << dyad.bytes << std::endl;
        }

        Quarry:: Vein:: Vein(Dyad &user) noexcept :
        ListOf<Stone>(),
        dyad(user)
        {
            std::cerr << "+Vein @2^" << dyad.shift << " = " << dyad.bytes << std::endl;
        }

        void * Quarry:: Vein:: acquire()
        {
            return (size>0) ? OutOfReach::Zero(popHead(),dyad.bytes) : dyad.acquire();
        }

        void Quarry:: Vein:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            pushHead( static_cast<Stone *>( OutOfReach::Zero(blockAddr,sizeof(Stone))) );
        }


    }

    namespace Memory
    {

        const char * const Quarry:: CallSign = "Memory::Quarry";

        Quarry:: ~Quarry() noexcept
        {
            for(unsigned shift=MaxShift;shift>=MinShift;--shift)
            {
                (void) OutOfReach::Naught(vein+shift);
            }
        }

        Quarry:: Quarry(Corpus &corpus) noexcept :
        vein(0),
        wksp()
        {
            vein = static_cast<Vein*>( Y_STATIC_ZARR(wksp) )-MinShift;
            for(unsigned shift=MinShift;shift<=MaxShift;++shift)
                new (vein+shift) Vein( corpus[shift] );
            
        }

        void * Quarry:: acquire(unsigned &shift)
        {
            if(shift>MaxShift) throw Specific::Exception(CallSign,"shift exceeds MaxShift=%u",MaxShift);
            const unsigned requested = Max(MinShift,shift);
            void *         blockAddr = vein[requested].acquire(); assert( OutOfReach::Are0(blockAddr, vein[requested].dyad.bytes) );
            shift = requested;
            return blockAddr;
        }


    }


}

