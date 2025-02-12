
#include "y/apex/api/ortho/family.hpp"


namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            void Family:: trim() noexcept
            {
                if(0!=qwork)
                {
                    cache->store(qwork);
                    qwork = 0;
                }
            }

            void Family:: free() noexcept
            {
                while(qlist.size>0)
                    cache->store(qlist.popHead());
            }


            Family:: ~Family() noexcept
            {
                trim();
                free();
            }

            Family:: Family(const Metrics &metrics, const Cache &sharedCache) noexcept :
            Metrics(metrics),
            Proxy<const Vector::List>(),
            qlist(),
            qwork(0),
            cache(sharedCache),
            next(0),
            prev(0)
            {
            }


            Quality Family:: getQuality() const noexcept
            {
                return  Metrics::getQuality( qlist.size );
            }

            const char * Family:: getQualityText() const noexcept {
                return Metrics::QualityText( getQuality() );
            }

            Y_PROXY_IMPL(Family,qlist)


            std::ostream & operator<<(std::ostream &os, const Family &f)
            {
                if(f->size<=0)
                {
                    os << "{}";
                }
                else
                {
                    os << '{' << std::endl;
                    for(const Vector *v=f->head;v;v=v->next)
                    {
                        os << '\t' << *v << std::endl;
                    }
                    os << '}' << std::endl;
                }
                return os;
            }
        }
    }
}
