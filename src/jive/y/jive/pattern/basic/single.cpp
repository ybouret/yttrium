
#include "y/jive/pattern/basic/single.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/io/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Single:: ~Single() noexcept
        {
        }

        
        Single:: Single(const uint8_t c) noexcept :
        Pattern(UUID), code(c)
        {
            Y_PATTERN(Single);
        }

        Pattern * Single:: clone() const
        {
            return new Single(code);
        }

        bool Single:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get(); if(!ch)   return false;
            if(code != **ch) { source.put(ch); return false; }
            return true;
        }

        void Single::query(FirstChars &fc) const
        {
            fc.include(code);
        }

        size_t Single:: serialize(OutputStream &fp) const
        {
            const size_t nw = fp.emitCBR(uuid);
            return nw + fp.emitCBR(code);
        }


    }

}
