
#include "y/jive/pattern/joker/guest.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Guest:: Guest(const uint32_t t,
                      const Pattern &source) :
        Pattern(t),
        motif( source.clone() )
        {
            assert(motif.isValid());
        }


        Guest:: Guest(const uint32_t t, Pattern *p) noexcept:
        Pattern(t),
        motif(p)
        {
            assert(motif.isValid());
        }

        Guest:: ~Guest() noexcept
        {

        }

        void Guest:: reset() noexcept
        {
            release();
            motif->reset();
        }

        Guest:: Guest(const Guest &other) :
        Pattern(other),
        motif(other.motif->clone())
        {

        }

        void Guest:: query(FirstChars &fc) const
        {
            assert(motif.isValid());
            motif->query(fc);
        }

        void Guest:: optimize()
        {
            assert(motif.isValid());
            motif = Pattern::Optimize( & *motif);
        }

        bool Guest:: hasSameMotifThan(const Guest &other) const noexcept
        {
            return motif->isEqualTo(*other.motif);
        }

        void Guest::vizLink(OutputStream &fp) const
        {
            motif->viz(fp);
            Endl(arrow(fp, & *motif));
        }
    }

}


