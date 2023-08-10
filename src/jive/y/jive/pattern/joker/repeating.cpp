
#include "y/jive/pattern/joker/repeating.hpp"
#include "y/io/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Repeating:: ~Repeating() noexcept
        {
        }

        static uint32_t RepUUID(const size_t nmin) noexcept
        {
            switch(nmin)
            {
                case 0: return Repeating::ZOM;
                case 1: return Repeating::OOM;
                default:
                    break;
            }
            return Repeating::UUID;

        }

        Repeating:: Repeating(const size_t nmin, const Pattern &source) :
        Guest(RepUUID(nmin),source),
        atLeast(nmin)
        {
            Y_PATTERN(Repeating);
        }

        Repeating:: Repeating(const size_t nmin, Pattern *source) :
        Guest(RepUUID(nmin),source),
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

        size_t Repeating:: serialize(OutputStream &fp) const
        {
            size_t nw = fp.emitCBR(uuid);
            switch(uuid)
            {
                case ZOM:
                case OOM:
                    break;
                default:
                    assert(UUID==uuid);
                    nw += fp.emitVBR(atLeast);
            }
            return nw + motif->serialize(fp);
        }

        bool Repeating:: isFragile() const noexcept
        {
            return atLeast <=0 || motif->isFragile();
        }
        

        bool Repeating:: takes(Source &source)
        {
            assert(0==size);
            assert(0==motif->size);

            


            return false;
        }

    }

}
