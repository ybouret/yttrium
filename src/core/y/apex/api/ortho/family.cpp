
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
                trim();
                while(qlist.size>0)
                    cache->store(qlist.popHead());
            }


            QFamily:: ~QFamily() noexcept
            {

            }

            QFamily:: QFamily(const Metrics &metrics, const QCache &sharedCache) noexcept :
            Metrics(metrics),
            Proxy<const QVector::List>(),
            qlist(),
            cache(sharedCache),
            next(0),
            prev(0)
            {
            }

            Y_PROXY_IMPL(QFamily,qlist)
            
        }
    }
}
