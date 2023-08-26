

#include "y/jive/pattern/joker/counting.hpp"
#include "y/stream/output.hpp"
#include "y/jive/source.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Counting:: ~Counting() noexcept
        {
        }

        
        Counting:: Counting(const size_t nmin, const size_t nmax, Pattern *source) :
        Guest(UUID,source),
        minCount(nmin),
        maxCount(nmax)
        {
            Y_PATTERN(Counting);
            if(minCount>maxCount) CoerceSwap(minCount, maxCount);
        }

        Pattern * Counting:: Make(const size_t nmin, const size_t nmax, Pattern *source)
        {
            assert(0!=source);
            AutoPtr<Pattern> guard(source);
            if(source->isFragile()) NoFragileMotif("Jive::Counting");
            Pattern *res = new Counting(nmin,nmax,source);
            (void) guard.yield();
            return res;
        }


        Pattern * Counting:: Make(const size_t nmin, const size_t nmax, const Pattern &source)
        {
            return Make(nmin,nmax,source.clone());
        }

        Counting:: Counting(const Counting &other)  :
        Guest(other),
        minCount(other.minCount),
        maxCount(other.maxCount)
        {
            Y_PATTERN(Counting);
        }

        Pattern * Counting:: clone() const
        {
            return new Counting(*this);
        }

        size_t Counting:: serialize(OutputStream &fp) const
        {
            size_t nw = fp.emitCBR(uuid);
            nw += fp.emitVBR(minCount);
            nw += fp.emitVBR(maxCount);
            return nw + motif->serialize(fp);
        }

        bool Counting:: isFragile() const noexcept
        {
            return minCount <= 0;
        }

        bool Counting:: takes(Source &source)
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

            if(count>=minCount&&count<=maxCount)
            {
                return true;
            }
            else
            {
                source.put(*this);
                return false;
            }

        }

        bool Counting:: isEqualTo(const Pattern &p) const noexcept
        {
            return
            UUID     == p.uuid &&
            minCount == p.as<Counting>()->minCount &&
            maxCount == p.as<Counting>()->maxCount &&
            hasSameMotifThan( *p.as<Counting>() );
        }


        void Counting:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            String msg = "cnt";
            Label(fp, msg) << ",shape=oval";
            fp << ']';
            Endl(fp);
            vizLink(fp);
        }

    }

}

