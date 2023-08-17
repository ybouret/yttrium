
#include "y/jive/pattern/basic.hpp"

namespace Yttrium
{
    namespace Jive
    {
        bool Pattern:: isBasic() const noexcept
        {
            switch (uuid)
            {
                case Single :: UUID:
                case Range  :: UUID:
                case Exclude:: UUID:
                    return true;

                default:
                    break;
            }
            return false;
        }
    }

}
