
#include "y/chem/species/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Actors:: ~Actors() noexcept {}
        Actors:: Actors() noexcept : BaseType() {}
        Actors:: Actors(const Actors &other) : BaseType(other) {}

        String Actors:: toString() const
        {
            String res;

            if(size>0)
            {
                const Actor *a = head;
                res += a->toString();
                for(a=a->next;a;a=a->next)
                {
                    res += '+';
                    res += a->toString();
                }
            }
            
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Actors &A)
        {
            const String s = A.toString();
            return os << s;
        }
    }


}

