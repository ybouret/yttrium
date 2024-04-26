
#include "y/ink/parallel/slab.hpp"
#include "y/memory/allocator/dyadic.hpp"

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
            static Memory::Dyadic &mgr = Memory::Dyadic::Instance();
            assert(0==wksp);
            assert(0==wlen);
            Coerce(wksp) = mgr.acquire(Coerce(wlen)=1,Request);
        }

        Slab:: ~Slab() noexcept
        {
            if(0!=wksp)
            {
                assert(wlen>0);
                static Memory::Dyadic &mgr = Memory::Dyadic::Location();
                mgr.release( Coerce(wksp), Coerce(wlen) );
            }
        }
    }

}
