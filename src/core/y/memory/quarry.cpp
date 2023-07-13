
#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/text/human-readable.hpp"

#include <iomanip>

namespace Yttrium
{

    namespace Memory
    {
        const char * const Quarry::Vein:: CallSign = "Memory::Quary::Vein";
        
        Quarry:: Vein:: ~Vein() noexcept
        {
            //std::cerr << "-Vein @2^" << dyad.shift << " = " << dyad.bytes << std::endl;
            release();
        }

        Quarry:: Vein:: Vein(Dyad &user) noexcept :
        PoolOf<Stone>(),
        dyad(user)
        {
            //std::cerr << "+Vein @2^" << dyad.shift << " = " << dyad.bytes << std::endl;
        }

        void * Quarry:: Vein:: acquire()
        {
            return (size>0) ? OutOfReach::Zero(query(),dyad.bytes) : dyad.acquire();
        }

        void Quarry:: Vein:: release(void *blockAddr) noexcept
        {
            assert(0!=blockAddr);
            store( static_cast<Stone *>( OutOfReach::Zero(blockAddr,sizeof(Stone))) );
        }

        void Quarry:: Vein:: release() noexcept
        {
            while(size>0) dyad.release( query() );
        }

        uint64_t Quarry:: Vein:: displayInfo(const size_t indent) const
        {
            uint64_t res = 0;
            if(size)
            {
                Core::Indent(std::cerr,indent) << "<" << CallSign << " blockSize='2^" << dyad.shift << " = " << dyad.bytes << "'>" << std::endl;
                Core::Indent(std::cerr,indent+2) << "available = " << std::setw(7) << size << std::endl;
                Core::Indent(std::cerr,indent+2) << "allocated = " << HumanReadable(res += size*dyad.bytes) << std::endl;
                Core::Indent(std::cerr,indent) << "<" << CallSign << "/>" << std::endl;
            }
            return res;
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


        void Quarry:: release(void *entry, const unsigned int shift) noexcept
        {
            assert(0!=entry);
            assert(shift>=MinShift);
            assert(shift<=MaxShift);
            vein[shift].release(entry);
        }


        void Quarry:: release() noexcept
        {
            for(unsigned shift=MaxShift;shift>=MinShift;--shift)
            {
                vein[shift].release();
            }
        }

        void Quarry:: gc(const size_t maxBytes) noexcept
        {
            
        }

        void  Quarry:: displayInfo(const size_t indent) const
        {
            uint64_t sum = 0;
            Core::Indent(std::cerr,indent) << "<" << CallSign << ">" << std::endl;
            for(unsigned shift=MinShift;shift<=MaxShift;++shift)
            {
                sum += vein[shift].displayInfo(indent+4);
            }
            Core::Indent(std::cerr,indent+2) << "totalAllocated : " << HumanReadable(sum) << std::endl;
            Core::Indent(std::cerr,indent) << "<" << CallSign << "/>" << std::endl;
        }

    }


}

