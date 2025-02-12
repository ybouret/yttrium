
#include "y/apex/api/ortho/families.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {


            void Families:: clear() noexcept
            {
                while(my.size>0)
                    fc->store(my.popTail());
            }

            Families:: ~Families() noexcept
            {
                clear();
            }

            Families:: Families(const Metrics &m, const FCache &c) noexcept :
            Metrics(m),
            my(),
            fc(c)
            {
            }

            Y_PROXY_IMPL(Families,my)
        }

    }

}

