

#include "y/jive/pattern/basic/range.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Range:: ~Range() noexcept
        {
        }


        Range:: Range(const uint8_t a, const uint8_t b) noexcept :
        Pattern(UUID), lower(a), upper(b)
        {
            Y_PATTERN(Range);
            if(lower>upper)
                CoerceSwap(lower,upper);
        }

        Range:: Range(const Range &other) noexcept :
        Pattern(other), lower(other.lower), upper(other.upper)
        {
            Y_PATTERN(Range);
        }


        Pattern * Range:: clone() const
        {
            return new Range(*this);
        }

        bool Range:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get(); if(!ch)   return false;
            const uint8_t code = **ch;
            if(code<lower||code>upper) { source.put(ch); return false; }
            pushTail(ch);
            return true;
        }

        void Range::query(FirstChars &fc) const
        {
            fc.include(lower,upper);
        }

        size_t Range:: serialize(OutputStream &fp) const
        {
            size_t nw = fp.emitCBR(uuid);
            nw += fp.emitCBR(lower);
            nw += fp.emitCBR(upper);
            return nw;
        }

        void Range:: reset() noexcept { release(); }

        bool Range:: isFragile() const noexcept
        {
            return false;
        }
        
        bool Range:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && lower == p.as<Range>()->lower && upper == p.as<Range>()->upper;
        }

        void Range:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            const char msg[8] = { '[', char(lower), '-', char(upper), ']',0,0,0 };
            Label(fp, msg,5) << ",shape=box";
            fp << ']';
            Endl(fp);
        }
    }

}
