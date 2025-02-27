#include "y/apex/api/ortho/coven/tribe.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
                {
                    const Posture &posture = tribe;
                    os << posture << "->" << *(tribe.qfamily);
                    return os;
                }
            }

        }
    }
}

