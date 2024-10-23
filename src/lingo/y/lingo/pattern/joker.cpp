
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

        void Joker:: query(CharDB &firstChars) const
        {
            assert(0!=motif);
            motif->query(firstChars);
        }

        const Pattern & Joker:: operator*() const noexcept
        {
            assert(0!=motif);
            return *motif;
        }

        void  Joker:: optimizing()
        {
            assert(0!=motif);
            Pattern * const result = Optimize(motif); assert(0!=result);
            if( result != motif )
            {
                delete motif;
                Coerce(motif) = result;
            }
        }


        void  Joker:: ignoreCase()
        {
            assert(0!=motif);
            Pattern * const result = IgnoreCase(motif); assert(0!=result);
            if( result != motif )
            {
                std::cerr << "Changing motif for ignoreCase" << std::endl;
                delete motif;
                Coerce(motif) = result;
                std::cerr << "done!" << std::endl;
            }
        }


    }

}
