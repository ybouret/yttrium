
#include "y/apex/api/ortho/family.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            Family:: Cache:: Cache(const Metrics &m, const VCache &c) noexcept:
            Metrics(m),
            my(),
            vc(c)
            {

            }

            Family:: Cache:: ~Cache() noexcept
            {

            }

            Y_PROXY_IMPL(Family::Cache,my)

            Family * Family:: Cache:: query()
            {
                return my.size > 0 ? my.query() : new Family(*this,vc);
            }

            void Family:: Cache:: store(Family * const F) noexcept
            {
                assert(0!=F);
                assert(dimensions==F->dimensions);
                my.store(F)->reset();
            }

            Family * Family:: Cache::query(const Family &F)
            {
                Family *D = query();

                try
                {
                    for(const Vector *v=F->head;v;v=v->next)
                    {
                        D->qlist.pushTail( vc->query(*v) );
                    }
                    Coerce(D->quality) = F.quality;
                    assert( AreEqual(F,*D) );
                    return D;
                }
                catch(...)
                {
                    store(D);
                    throw;
                }
            }

        }

    }

}

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {

            void Family:: prune() noexcept
            {
                if(0!=qwork)
                {
                    cache->store(qwork);
                    qwork = 0;
                }
            }

            void Family:: clear() noexcept
            {
                while(qlist.size>0)
                    cache->store(qlist.popHead());
                Coerce(quality) = getQuality(0);
            }

            void Family:: reset() noexcept
            {
                clear();
                prune();
            }

            Family:: ~Family() noexcept
            {
                reset();
            }

            Family:: Family(const Metrics &m, const VCache &c) noexcept :
            Metrics(m),
            Proxy<const Vector::List>(),
            quality( getQuality(0) ),
            qlist(),
            qwork(0),
            cache(c),
            next(0),
            prev(0)
            {
            }


            
            Y_PROXY_IMPL(Family,qlist)

            bool Family:: AreEqual(const Family &lhs, const Family &rhs) noexcept
            {
                assert(lhs.dimensions==rhs.dimensions);
                if(lhs.qlist.size!=rhs.qlist.size)
                {
                    assert(lhs.quality!=rhs.quality);
                    return false;
                }
                else
                {
                    assert(lhs.quality==rhs.quality);
                    for(const Vector *l=lhs.qlist.head, *r=rhs.qlist.head;l;l=l->next,r=r->next)
                    {
                        assert(0!=l);
                        assert(0!=r);
                        if( __Zero__ != Vector::Compare(*l,*r) ) return false;
                    }
                    return true;
                }
            }

            bool operator==(const Family &lhs, const Family &rhs) noexcept
            {
                return Family::AreEqual(lhs, rhs);
            }

            bool operator!=(const Family &lhs, const Family &rhs) noexcept
            {
                return !Family::AreEqual(lhs, rhs);
            }

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

            void Family:: expand()  
            {
                assert(0!=qwork);
                assert(qwork->ncof>0);
                assert(qwork->nrm2>0);
                assert(qlist.size<dimensions);

                qlist.pushTail(qwork);
                Coerce(quality) = getQuality(qlist.size);
                qwork = 0;
                {
                    Vector * const curr = qlist.tail;
                LOOP:
                    if(0!=curr->prev)
                        switch( Vector::Compare(*(curr->prev),*curr) )
                        {
                            case __Zero__: throw Specific::Exception("Ortho::Family", "unexpected multiple vectors!!");
                            case Negative: return;
                            case Positive: qlist.towardsHead(curr); goto LOOP;
                        }
                }


            }
        }
    }
}
