

#include "y/jive/pattern/basic/range.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"


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

        Pattern * Range:: clone() const
        {
            return new Range(lower,upper);
        }

        bool Range:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get(); if(!ch)   return false;
            const uint8_t code = **ch;
            if(code<lower||code>upper) { source.put(ch); return false; }
            return true;
        }

        void Range::query(FirstChars &fc) const
        {
            fc.include(lower,upper);
        }


    }

}
