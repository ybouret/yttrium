
#include "y/jive/pattern/basic/single.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/stream/output.hpp"

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

        Single:: Single(const Single &other) noexcept :
        Pattern(other), code(other.code)
        {
            Y_PATTERN(Single);
        }



        Pattern * Single:: clone() const
        {
            return new Single(*this);
        }
        

        bool Single:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get(); if(!ch)   return false;
            if(code != **ch) { source.put(ch); return false; }
            pushTail(ch);
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

        void Single:: reset() noexcept { release(); }


        bool Single:: isFragile() const noexcept
        {
            return false;
        }

        bool Single:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && code == p.as<Single>()->code;
        }

        void Single:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            Label(fp, &code, 1) << ",shape=box";
            fp << ']';
            Endl(fp);
        }

        bool Single:: isUnivocal() const noexcept
        {
            return true;
        }

    }

}
