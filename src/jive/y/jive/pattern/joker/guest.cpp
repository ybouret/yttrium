
#include "y/jive/pattern/joker/guest.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Guest:: Guest(const uint32_t t, const Pattern &source) :
        Pattern(t),
        motif( source.clone() )
        {
        }


        Guest:: Guest(const uint32_t t, Pattern *p) :
        Pattern(t),
        motif(p)
        {
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
        motif(other.motif)
        {

        }

        void Guest:: query(FirstChars &fc) const
        {
            motif->query(fc);
        }

        void Guest:: optimize()
        {
            ClonePtr<Pattern> opt( Pattern::Optimize( & *motif) );
        }

        bool Guest:: hasSameMotifThan(const Guest &other) const noexcept
        {
            return motif->isEqualTo(*other.motif);
        }

        

    }

}


