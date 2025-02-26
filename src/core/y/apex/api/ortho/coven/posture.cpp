
#include "y/apex/api/ortho/coven/posture.hpp"
namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                std::ostream & operator<<(std::ostream &os, const Posture &self)
                {
                    os << self.content << ':' << self.residue;
                    return os;
                }
            }

        }

    }
}


