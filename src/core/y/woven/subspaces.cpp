
#include "y/woven/subspaces.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        Subspaces :: ~Subspaces() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Subspaces &subs)
        {
            os << '{' << std::endl;
            for(const Subspace *s=subs.head;s;s=s->next)
            {
                os << "  " << *s << std::endl;
            }
            os << '}';
            return os;
        }
        
    }

}
