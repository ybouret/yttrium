
#include "y/jive/pattern/joker/repeating.hpp"
#include "y/stream/output.hpp"
#include "y/jive/source.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Repeating:: ~Repeating() noexcept
        {
        }


        Pattern * Repeating:: Make(const size_t nmin, Pattern *p)
        {
            assert(0!=p);
            AutoPtr<Pattern> guard(p);
            if(p->isFragile()) NoFragileMotif("Jive::Repeating");
            Pattern *rep = new Repeating(nmin,p);
            guard.yield();
            return rep;
        }

        Pattern * Repeating:: Make(const size_t nmin, const Pattern &P)
        {
            return Make(nmin,P.clone());
        }
        

        Repeating:: Repeating(const size_t nmin, Pattern *source) :
        Guest(UUID,source),
        atLeast(nmin)
        {
            Y_PATTERN(Repeating);
        }

        Repeating:: Repeating(const Repeating &other)  :
        Guest(other),
        atLeast(other.atLeast)
        {
            Y_PATTERN(Repeating);
        }

        Pattern * Repeating:: clone() const
        {
            return new Repeating(*this);
        }




        const uint32_t Repeating:: UUID;
        const uint32_t Repeating:: ZOM;
        const uint32_t Repeating:: OOM;


        size_t Repeating:: serialize(OutputStream &fp) const
        {
            size_t nw = 0;
            switch(atLeast)
            {
                case 0: nw += fp.emitCBR(ZOM); break;
                case 1: nw += fp.emitCBR(OOM); break;
                default:
                    nw += fp.emitCBR(UUID);
                    nw += fp.emitVBR(atLeast);
                    break;
            }
            return nw + motif->serialize(fp);
        }

        bool Repeating:: isFragile() const noexcept
        {
            return atLeast <=0;
        }
        

        bool Repeating:: takes(Source &source)
        {
            assert(0==size);
            assert(0==motif->size);

            size_t count = 0;
        PROBE:
            assert(0==motif->size);
            if(motif->takes(source))
            {
                assert(motif->size>0);
                ++count;
                mergeTail(*motif);
                goto PROBE;
            }

            if(count>=atLeast)
            {
                return true;
            }
            else
            {
                source.put(*this);
                return false;
            }

        }

        bool Repeating:: isEqualTo(const Pattern &p) const noexcept
        {
            return
            UUID    == p.uuid &&
            atLeast == p.as<Repeating>()->atLeast
            && hasSameMotifThan( *p.as<Repeating>() );
        }

        Pattern * Repeating::ZeroOrMore(Pattern *p)
        {
            return Repeating::Make(0,p);
        }

        Pattern * Repeating::ZeroOrMore(const Pattern &p)
        {
            return Repeating::Make(0,p);
        }

        Pattern * Repeating:: OneOrMore(Pattern *p)
        {
            return Repeating::Make(1,p);
        }

        Pattern * Repeating::OneOrMore(const Pattern &p)
        {
            return Repeating::Make(1,p);
        }


        void Repeating:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            switch(atLeast)
            {
                case 0: Label(fp,"*"); break;
                case 1: Label(fp,"+"); break;
                default:
                {
                    const String msg = FormatString(">=%u",unsigned(atLeast));
                    Label(fp,msg);
                }
            }
            fp << ",shape=circle";
            fp << ']';
            Endl(fp);
            vizLink(fp);
        }

    }

}
