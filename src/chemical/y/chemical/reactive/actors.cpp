
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

    }

}
