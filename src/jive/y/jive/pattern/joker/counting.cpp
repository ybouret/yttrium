

#include "y/jive/pattern/joker/counting.hpp"
#include "y/io/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Counting:: ~Counting() noexcept
        {
        }

        Counting:: Counting(const size_t nmin, const size_t nmax, const Pattern &source) :
        Guest(UUID,source),
        minCount(nmin),
        maxCount(nmax)
        {
            Y_PATTERN(Counting);
            if(minCount>maxCount) CoerceSwap(minCount, maxCount);
        }

        Counting:: Counting(const size_t nmin, const size_t nmax, Pattern *source) :
        Guest(UUID,source),
        minCount(nmin),
        maxCount(nmax)
        {
            Y_PATTERN(Counting);
            if(minCount>maxCount) CoerceSwap(minCount, maxCount);
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
            return minCount <= 0 || motif->isFragile();
        }

        bool Counting:: takes(Source &source)
        {
            assert(0==size);
            assert(0==motif->size);


            return false;
        }

        bool Counting:: isEqualTo(const Pattern &p) const noexcept
        {
            return
            UUID     == p.uuid &&
            minCount == p.as<Counting>()->minCount &&
            maxCount == p.as<Counting>()->maxCount &&
            hasSameMotifThan( *p.as<Counting>() );
        }

    }

}

