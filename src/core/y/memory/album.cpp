#include "y/memory/album.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/lockable.hpp"
#include "y/text/human-readable.hpp"

namespace Yttrium
{


    namespace Memory
    {

        const char * const  Album::CallSign = "Memory::Album";

        void Album:: setupWith(Lockable &giant) noexcept
        {
            pages = static_cast<Pages*>( OutOfReach::Zero(pages_,sizeof(pages_)) );
            pages -= Pages::MinShift;
            for(unsigned shift=Pages::MinShift;shift<=Pages::MaxShift;++shift)
            {
                new ( &pages[shift] ) Pages(shift,giant);
                assert(pages[shift].bytes == (Base2<size_t>::One << shift) );
            }
        }


        Album:: Album() : pages(NULL), pages_() { setupWith(Lockable::Giant()); }

        Album:: Album(Lockable &giant) noexcept : pages(NULL), pages_() { setupWith(giant); }


        Album:: ~Album() noexcept
        {
            for(unsigned shift=Pages::MaxShift;shift>=Pages::MinShift;--shift)
            {
                (void) OutOfReach::Naught( &pages[shift] );
            }
        }

        Pages & Album:: operator[](const unsigned shift) noexcept
        {
            assert(shift>=Pages::MinShift);
            assert(shift<=Pages::MaxShift);
            assert( pages[shift].bytes == (Base2<size_t>::One << shift) );
            return pages[shift];
        }

        void Album:: displayInfo(const size_t indent) const
        {
            Core::Indent(std::cerr,indent) << '<' << CallSign << '>' << std::endl;

            const size_t pageIndent = indent+4;
            uint64_t     allocated  = 0;

            for(unsigned shift=Pages::MinShift;shift<=Pages::MaxShift;++shift)
            {
                allocated += pages[shift].displayInfo(pageIndent);
            }

            Core::Indent(std::cerr,indent) << "  allocated = " << HumanReadable(allocated) << std::endl;
            Core::Indent(std::cerr,indent) << '<' << CallSign << '/' << '>' << std::endl;
        }

    }

}

