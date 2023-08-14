#include "y/jive/pattern/logic/compound.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"

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
                patterns.pushTail( Pattern::ReadFrom(fp) );
        }

        Compound & Compound:: operator<<( const Pattern &p )
        {
            patterns.pushTail( p.clone() );
            return *this;
        }

        Compound & Compound:: operator<<( Pattern *p )
        {
            assert(0!=p);
            patterns.pushTail(p);
            return *this;
        }

        bool Compound:: hasSamePatternsThan(const Compound &other) const noexcept
        {
            if(patterns.size != other.patterns.size) return false;

            for(const Pattern *lhs=patterns.head, *rhs=other.patterns.head;lhs;lhs=lhs->next,rhs=rhs->next)
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if( ! lhs->isEqualTo(*rhs) ) return false;
            }
            return true;
        }

        void Compound:: vizLink(OutputStream &fp) const
        {
            for(const Pattern *p=patterns.head;p;p=p->next)
            {
                p->viz(fp);
                Endl(arrow(fp, & *p));
            }
        }

    }

}

#include "y/jive/pattern/basic/single.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    namespace Jive
    {

        void Compound:: feed(const char *buf, const size_t len)
        {
            assert( Good(buf,len) );
            for(size_t i=0;i<len;++i)
            {
                patterns.pushTail( new Single(buf[i]) );
            }
        }

        void Compound:: feed(const char *buf)
        {
            feed(buf, StringLength(buf) );
        }


        void Compound:: feed(const String &s)
        {
            feed( s(), s.size() );
        }
    }

}
