
#include "y/jive/pattern/joker/guest.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {


        void Guest:: NoFragileMotif(const char *clid)
        {
            assert(0!=clid);
            throw Specific::Exception(clid,"Fragile pattern is not accepted");
        }

        Guest:: Guest(const uint32_t t,
                      const Pattern &source) :
        Pattern(t),
        motif( source.clone() )
        {
            assert(motif.isValid());
            assert(motif->isRegular());
        }


        Guest:: Guest(const uint32_t t, Pattern *p) noexcept:
        Pattern(t),
        motif(p)
        {
            assert(motif.isValid());
            assert(motif->isRegular());
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
            assert(motif.isValid());
            assert(motif->isRegular());
        }

        void Guest:: query(FirstChars &fc) const
        {
            assert(motif.isValid());
            motif->query(fc);
        }

        void Guest:: optimize()
        {
            assert(motif.isValid());
            assert(motif->isRegular());
            motif = Pattern::Optimize( motif.yield() );
            assert(motif.isValid());
            assert(motif->isRegular());
        }

        void Guest:: ignoreCase()
        {
            assert(motif.isValid());
            assert(motif->isRegular());
            motif = Pattern::IgnoreCase( motif.yield() );
            assert(motif.isValid());
            assert(motif->isRegular());
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


