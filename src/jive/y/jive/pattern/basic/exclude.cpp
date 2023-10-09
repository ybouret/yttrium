

#include "y/jive/pattern/basic/exclude.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Exclude:: ~Exclude() noexcept
        {
        }


        Exclude:: Exclude(const uint8_t c) noexcept :
        Pattern(UUID), code(c)
        {
            Y_PATTERN(Exclude);
        }

        Exclude:: Exclude(const Exclude &other) noexcept :
        Pattern(other), code(other.code)
        {
            Y_PATTERN(Exclude);
        }



        Pattern * Exclude:: clone() const
        {
            return new Exclude(*this);
        }


        bool Exclude:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get(); if(!ch)   return false;
            if(code == **ch) { source.put(ch); return false; }
            pushTail(ch);
            return true;
        }

        void Exclude::query(FirstChars &fc) const
        {
            fc.includeAll();
            fc.exclude(code);
        }

        size_t Exclude:: serialize(OutputStream &fp) const
        {
            const size_t nw = fp.emitCBR(uuid);
            return nw + fp.emitCBR(code);
        }

        void Exclude:: reset() noexcept { release(); }

        bool Exclude:: isFragile() const noexcept
        {
            return false;
        }

        bool Exclude:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && code == p.as<Exclude>()->code;
        }

        void Exclude:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp, &code, 1) << ",shape=invtriangle";
            fp << ']';
            Endl(fp);
        }

        bool Exclude:: isUnivocal() const noexcept
        {
            return false;
        }
    }

}
