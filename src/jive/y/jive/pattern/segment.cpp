
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


        std::ostream & operator<<(std::ostream &os, const Segment &seg)
        {
            seg.display(os);
            return os;
        }


        Segment * Segment:: Optimize(Segment *seg)
        {
            assert(0!=seg);
            switch(seg->uuid)
            {
                case Ponctual::UUID: return seg;
                case Interval::UUID:
                    break;
            }
            assert(0!=seg->self);
            Interval &cpl = *seg->as<Interval>();
            if(cpl.lower>=cpl.upper)
            {
                // couple->single
                const AutoPtr<Segment> keep(seg);
                return new Ponctual(cpl.lower);
            }
            else
            {
                // regular couple
                return seg;
            }
        }


        Segment *TryMergeSpliced(Ponctual *lhs, Interval *rhs)
        {
            assert(0!=rhs);
            assert(0!=lhs);
            const uint8_t lower = rhs->lower;
            const uint8_t upper = rhs->upper;
            const uint8_t value = lhs->value;

            if(value<lower)
            {
                if(1==lower-value)
                {
                    // adjacent
                    delete lhs; --Coerce(rhs->lower); return rhs;
                }
                else
                    // separated
                    return 0;
            }
            else
            {
                if(value>upper)
                {
                    if(1==value-upper)
                    {
                        // adjacent
                        delete lhs; ++Coerce(rhs->upper); return rhs;
                    }
                    else
                        // separatd
                        return 0;
                }
                else
                {
                    delete lhs;
                    return rhs;
                }
            }

        }

        Segment *TryMergeCouples(Interval *lhs, Interval *rhs)
        {

            return 0;
        }

        Segment *TryMergeSingles(Ponctual *lhs, Ponctual *rhs)
        {
            uint8_t lower = lhs->value;
            uint8_t upper = rhs->value;
            if(lower>upper) Swap(lower,upper);
            assert(lower<=upper);

            switch(upper-lower)
            {
                case 0: {
                    // same
                    const AutoPtr<Segment> L(lhs), R(rhs);
                    return new Ponctual(lower);
                }

                case 1: {
                    // difference=1
                    const AutoPtr<Segment> L(lhs), R(rhs);
                    return new Interval(lower,upper);
                }

                default:
                    // separated
                    break;
            }

            return 0; // do nothing
        }

        Segment * Segment:: TryMerge(Segment *lhs, Segment *rhs)
        {
            assert(0!=lhs);
            assert(0!=rhs);
            if(lhs->uuid==Ponctual::UUID)
            {
                if(rhs->uuid==Ponctual::UUID) return TryMergeSingles( lhs->as<Ponctual>(), rhs->as<Ponctual>());
                if(rhs->uuid==Interval::UUID) return TryMergeSpliced( lhs->as<Ponctual>(), rhs->as<Interval>() );
            }

            if(lhs->uuid==Interval::UUID)
            {
                if(rhs->uuid==Ponctual::UUID) return TryMergeSpliced(rhs->as<Ponctual>(), lhs->as<Interval>() );
                if(rhs->uuid==Interval::UUID) return TryMergeCouples(lhs->as<Interval>(), rhs->as<Interval>());
            }

            return 0;
        }

    }
}

namespace Yttrium
{
    namespace Jive
    {
        Ponctual:: Ponctual(const uint8_t code) noexcept :
        Segment(UUID),
        value(code)
        {
            self = (Ponctual *)this;
        }

        Ponctual:: ~Ponctual() noexcept
        {

        }

        bool Ponctual:: contains(const uint8_t code) const noexcept
        {
            return code==value;
        }

        void Ponctual:: display(std::ostream &os) const
        {
            os << '{' << char(value) << '}';
        }
    }

}

namespace Yttrium
{
    namespace Jive
    {
        Interval:: Interval(const uint8_t lo, const uint8_t hi) noexcept :
        Segment(UUID),
        lower(lo),
        upper(hi)
        {
            assert(lower<=upper);
            self = (Interval *)this;
        }

        Interval:: ~Interval() noexcept
        {

        }

        bool Interval:: contains(const uint8_t code) const noexcept
        {
            return code>=lower && code <=upper;

        }

        void Interval:: display(std::ostream &os) const
        {
            os << '[' << char(lower) << '-' << char(upper) << ']';
        }
    }

}

