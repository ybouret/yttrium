#include "y/jive/pattern/segment.hpp"
#include "y/text/ascii/printable.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Segment::Segment(const uint8_t a, const uint8_t b) noexcept :
        Object(),
        next(0),
        prev(0),
        lower(a),
        upper(b)
        {
            if(lower>upper) CoerceSwap(lower,upper);
        }

        Segment::Segment(const uint8_t a) noexcept :
        Object(),
        next(0),
        prev(0),
        lower(a),
        upper(a)
        {
        }

        Segment:: ~Segment() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Segment &s)
        {
            os << '[';
            os << ASCII::Printable::Char[s.lower];
            if(s.upper>s.lower) os << '-' << ASCII::Printable::Char[s.upper];
            os << ']';
            return os;
        }

        bool Segment:: contains(const uint8_t code) noexcept
        {
            return code>=lower && code<=upper;
        }

        static inline
        Segment *TryMerge(Segment *lhs, Segment *rhs) noexcept
        {
            return 0;
        }

        void Segment:: AppendTo(List &source, Segment *rhs) noexcept
        {
            assert(0!=rhs);
            std::cerr << "appending " << *rhs << " to " << source << std::endl;
            {
                List target;
                while(source.size)
                {
                    Segment *lhs = source.popHead();
                    Segment *res = TryMerge(lhs,rhs);
                    if(res)
                    {
                        rhs = res;
                    }
                    else
                    {
                        target.pushTail(lhs);
                    }
                }
                source.swapWith(target);
            }

            if(source.size<=0) {  }

            switch(source.size)
            {
                case 0: source.pushTail(rhs); return;
                case 1:
                    return;
                default:
                    break;
            }



        }


    }

}

