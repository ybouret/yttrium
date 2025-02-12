
#include "y/apex/api/ortho/family.hpp"


namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            void QFamily:: trim() noexcept
            {
                if(0!=qwork)
                {
                    cache->store(qwork);
                    qwork = 0;
                }
            }

            void QFamily:: free() noexcept
            {
                while(qlist.size>0)
                    cache->store(qlist.popHead());
            }


            QFamily:: ~QFamily() noexcept
            {
                trim();
                free();
            }

            QFamily:: QFamily(const Metrics &metrics, const Cache &sharedCache) noexcept :
            Metrics(metrics),
            Proxy<const Vector::List>(),
            qlist(),
            qwork(0),
            cache(sharedCache),
            next(0),
            prev(0)
            {
            }

            Y_PROXY_IMPL(QFamily,qlist)
            
        }
    }
}
