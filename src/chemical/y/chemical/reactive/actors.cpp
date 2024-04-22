
#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Actors:: Actors() noexcept : CxxListOf<Actor>() {}
        Actors:: ~Actors() noexcept {}
        Actors:: Actors(const Actors &A) : CxxListOf<Actor>(A) {}
        

        String Actors:: toString() const
        {
            String res;
            const Actor *a  = head;
            if(a)
            {
                res += a->asFirst();
                while( 0 != (a=a->next) )
                {
                    res += a->asExtra();
                }
            }
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Actors &A)
        {
            return (os << A.toString());
        }

        int Actors:: charge() const noexcept
        {
            int res = 0;
            for(const Actor *a=head;a;a=a->next) res += a->charge();
            return res;
        }

        bool Actors:: contains(const Species &sp) const noexcept
        {
            for(const Actor *a=head;a;a=a->next)
            {
                if( &(a->sp) == &sp) return true;
            }
            return false;
        }

    }

}
