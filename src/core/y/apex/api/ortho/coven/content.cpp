#include "y/apex/api/ortho/coven/content.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            namespace Coven
            {
                Y_PROXY_IMPL(Content,my)

                bool operator==(const Content &lhs, const Content &rhs) noexcept
                {
                    return IList::AreEqual(lhs.my,rhs.my);
                }
            }
        }


    }

}

