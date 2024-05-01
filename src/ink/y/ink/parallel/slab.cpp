
#include "y/ink/parallel/slab.hpp"
#include "y/ink/types.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Ink
    {
        Slab:: Slab(const ThreadContext &ctx) noexcept :
        Concurrent::Frame2D<unit_t>(ctx),
        hseg( *(const HSegment **) Memory::OutOfReach::Addr(&segment) ),
        wksp(),
        wlen()
        {
        }

        void Slab:: honorRequest()
        {
            static Memory::Allocator &mgr = MemoryModel::Instance();
            assert(0==wksp);
            assert(0==wlen);
            Coerce(wksp) = mgr.acquire(Coerce(wlen)=1,Request);
        }

        Slab & Slab:: ldz() noexcept
        {
            (void) memset(wksp,0,Request);
            return *this;
        }

        Slab:: ~Slab() noexcept
        {
            if(0!=wksp)
            {
                assert(wlen>0);
                static Memory::Allocator &mgr = MemoryModel::Location();
                mgr.release( Coerce(wksp), Coerce(wlen) );
            }
        }

        void Slab:: checkRequest(const size_t n, const size_t bs) const
        {
            const size_t numBytes = n * bs;
            if(numBytes>wlen)
                throw Specific::Exception("Ink::Slab", "insufficient local memory");
        }

    }

}
