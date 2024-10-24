
#include "y/lingo/pattern/joker.hpp"
#include "y/system/exception.hpp"


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
            std::cerr << "In Joker " << callSign() << std::endl;
            assert(0!=motif);
            Pattern * const result = Optimize(motif);
            assert(0!=result);
            std::cerr << "Ok " << result->callSign() << std::endl;
            if( result != motif )
            {
                std::cerr << "deleting " << motif->callSign() << std::endl;
                delete motif;
                std::cerr << "replacing with" << result->callSign() << std::endl;
                Coerce(motif) = result;
                std::cerr << "replaced with " << motif->callSign() << std::endl;
            }

            assert(0!=motif);
        }


        void  Joker:: ignoreCase()
        {
            assert(0!=motif);
            Coerce(motif) = IgnoreCase(motif);
            assert(0!=motif);
        }

        void Joker:: CheckStrong(const char * const     host,
                                 const Pattern * const guest)
        {

            assert(0!=host);
            assert(0!=guest);
            if(guest->strong()) return;
            throw Specific::Exception(host, "guest '%s' is not strong",guest->callSign());
        }

    }

}
