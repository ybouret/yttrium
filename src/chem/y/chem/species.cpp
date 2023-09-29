
#include "y/chem/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Species:: ~Species() noexcept {}
        
        String Species:: toString() const
        {
            String res;
            res += '[';
            res += name;
            res += ']';
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Species &s)
        {
            os << s.toString();
            return os;
        }

    }

}

