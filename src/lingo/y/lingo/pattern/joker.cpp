
#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Joker:: ~Joker() noexcept
        {
            assert(0!=motif);
            delete motif;
            Coerce(motif) = 0;
        }

        Joker:: Joker(const uint32_t t, Pattern *m) noexcept :
        Pattern(t),
        motif(m)
        {
            assert(0!=m);
        }

        size_t Joker:: serialize(OutputStream &fp) const
        {
            const size_t ans = emitUUID(fp);
            return ans+motif->serialize(fp);
        }

        void Joker:: query(CharDB &firstChars) const
        {
            assert(0!=motif);
            motif->query(firstChars);
        }


    }

}
