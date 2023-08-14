
#include "y/jive/pattern/joker/optional.hpp"
#include "y/stream/output.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Optional:: ~Optional() noexcept
        {
        }

        Pattern * Optional::From(Pattern *p)
        {
            assert(0!=p);
            AutoPtr<Pattern> guard(p);
            Pattern *opt = new Optional(p);
            guard.yield();
            return opt;
        }

        Pattern * Optional::From(const Pattern &P)
        {
            return From(P.clone());
        }

        Optional:: Optional(Pattern *source) :
        Guest(UUID,source)
        {
            Y_PATTERN(Optional);
        }

        Optional:: Optional(const Optional &other)  :
        Guest(other)
        {
            Y_PATTERN(Optional);
        }

        Pattern * Optional:: clone() const
        {
            return new Optional(*this);
        }

        size_t Optional:: serialize(OutputStream &fp) const
        {
            const size_t nw = fp.emitCBR(uuid);
            return nw + motif->serialize(fp);
        }

        bool Optional:: isFragile() const noexcept { return true; }

        bool Optional:: takes(Source &source)
        {
            assert(0==size);
            assert(0==motif->size);

            if(motif->takes(source))
                motif->swapWith(*this);
            
            return true;
        }

        bool Optional:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && hasSameMotifThan( *p.as<Optional>() );
        }


    }

}

