#include "y/jive/pattern/logic/compound.hpp"
#include "y/io/stream/output.hpp"
#include "y/io/stream/input.hpp"
//#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Compound:: ~Compound() noexcept
        {
        }

        Compound:: Compound(const uint32_t t) noexcept : Pattern(t), patterns() {}

        Compound:: Compound(const Compound &other) :
        Pattern(other),
        patterns(other.patterns)
        {
        }


        void Compound:: reset() noexcept
        {
            release();
            for(Pattern *p=patterns.head;p;p=p->next) p->reset();
        }

        size_t Compound:: serialize(OutputStream &fp) const
        {
            size_t nw = fp.emitCBR(uuid);
            nw += fp.emitVBR(patterns.size);
            for(const Pattern *p=patterns.head;p;p=p->next)
            {
                nw += p->serialize(fp);
            }
            return nw;
        }

        void Compound:: retrieve(InputStream &fp)
        {
            const size_t n = fp.readVBR<size_t>("Compound.patterns");
            for(size_t i=1;i<=n;++i)
                Coerce(patterns).pushTail( Pattern::ReadFrom(fp) );
        }


    }

}
