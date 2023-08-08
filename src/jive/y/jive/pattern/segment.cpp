
#include "y/jive/pattern/segment.hpp"
#include "y/ptr/auto.hpp"
namespace Yttrium
{
    namespace Jive
    {
        Segment:: Segment(const uint32_t t) noexcept :
        Object(),
        next(0),
        prev(0),
        self(0),
        uuid(t)
        {
        }

        Segment:: ~Segment() noexcept
        {
        }


        Segment * Segment:: Optimize(Segment *seg)
        {
            assert(0!=seg);
            switch(seg->uuid)
            {
                case Single::UUID: return seg;
                case Couple::UUID:
                    break;
            }
            assert(0!=seg->self);
            Couple &cpl = *static_cast<Couple *>(seg->self);
            if(cpl.lower>=cpl.upper)
            {
                // couple->single
                const AutoPtr<Segment> keep(seg);
                return new Single(cpl.lower);
            }
            else
            {
                // regular couple
                return seg;
            }
        }
    }
}

namespace Yttrium
{
    namespace Jive
    {
        Single:: Single(const uint8_t code) noexcept :
        Segment(UUID),
        value(code)
        {
            self = (Single *)this;
        }

        Single:: ~Single() noexcept
        {

        }

        bool Single:: contains(const uint8_t code) const noexcept
        {
            return code==value;

        }
    }

}

namespace Yttrium
{
    namespace Jive
    {
        Couple:: Couple(const uint8_t lo, const uint8_t hi) noexcept :
        Segment(UUID),
        lower(lo),
        upper(hi)
        {
            assert(lower<=upper);
            self = (Couple *)this;
        }

        Couple:: ~Couple() noexcept
        {

        }

        bool Couple:: contains(const uint8_t code) const noexcept
        {
            return code>=lower && code <=upper;

        }
    }

}

