
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
                Coerce(quality) = getQuality(0);
            }


            Family:: ~Family() noexcept
            {
                trim();
                free();
            }

            Family:: Family(const Metrics &metrics, const Cache &sharedCache) noexcept :
            Metrics(metrics),
            Proxy<const Vector::List>(),
            quality( getQuality(0) ),
            qlist(),
            qwork(0),
            cache(sharedCache),
            next(0),
            prev(0)
            {
            }


            
            Y_PROXY_IMPL(Family,qlist)


            std::ostream & operator<<(std::ostream &os, const Family &f)
            {

                os << "{<" << Metrics::QualityText(f.quality)  << ">";
                if( f->size> 0)
                {
                    os << std::endl;
                    unsigned i = 1;
                    for(const Vector *v=f->head;v;v=v->next,++i)
                    {
                        os << '\t' << "e" << i << "=" << *v << std::endl;
                    }
                }
                os << "}";
                return os;
            }

            void Family:: expand() noexcept
            {
                assert(0!=qwork);
                assert(qwork->ncof>0);
                assert(qwork->nrm2>0);
                assert(qlist.size<dimensions);

                qlist.pushTail(qwork);
                qwork = 0;
                Coerce(quality) = getQuality(qlist.size);
            }
        }
    }
}
