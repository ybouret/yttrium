
#include "y/apex/api/ortho/family.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {
            Family:: Cache:: ~Cache() noexcept
            {
            }

            Family:: Cache:: Cache(const VCache &_) noexcept :
            Object(),
            Counted(),
            Proxy<const Pool>(),
            my(),
            vcache(_)
            {
            }

            Y_PROXY_IMPL(Family::Cache, my)

            Family * Family:: Cache:: query()
            {
                return my.size>0 ? my.query() : new Family(vcache);
            }

            void Family:: Cache:: store(Family *const F) noexcept
            {
                assert(0!=F);
                assert(F->dimensions == vcache->dimensions);
                my.store(F)->reset();
            }

            Family * Family:: Cache:: query(const Family &F)
            {
                Family *res = query();
                try {
                    res->recreate(F);
                    return res;
                }
                catch(...)
                {
                    store(res);
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
                norm2           = 0;
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

            Family:: Family(const VCache &c) noexcept :
            Metrics(*c),
            Proxy<const Vector::List>(),
            quality( getQuality(0) ),
            qlist(),
            norm2(0),
            qwork(0),
            cache(c),
            next(0)
            {
            }

#if 0
            Family:: Family(const Family &_) :
            Metrics(_),
            Proxy<const Vector::List>(),
            quality(_.quality),
            qlist(),
            norm2(_.norm2),
            qwork(0),
            cache(_.cache),
            next(0)
            {
                for(const Vector *v=_->head;v;v=v->next)
                {
                    qlist.pushTail( cache->query(*v) );
                }
            }
#endif


            
            Y_PROXY_IMPL(Family,qlist)

            void Family:: recreate(const Family &F)
            {
                assert(this != &F);
                reset();
                try {
                    norm2 = F.norm2;
                    for(const Vector *src=F->head;src;src=src->next)
                        qlist.pushTail( cache->query(*src) );
                    Coerce(quality) = F.quality;
                }
                catch(...)
                {
                    reset();
                    throw;
                }
            }
            
            bool Family:: includes(const Family &sub)
            {
                if(sub->size>qlist.size) return false;
                for(const Vector *v=sub->head;v;v=v->next)
                {
                    if( welcomes(*v) )
                        return false; // vector not in my space
                }
                return true;
            }

            

            bool Family:: isAnalogousTo(const Family &sub)
            {
                if(qlist.size!=sub->size)
                    return false;
                else
                {
                     
                    for(const Vector *vec=sub->head;vec;vec=vec->next)
                    {
                        if( welcomes(*vec) )
                            return false; // vector not in my space
                    }
                    return true;
                }
            }

            bool Family:: isIdenticalTo(const Family &sub)
            {
                if(qlist.size!=sub->size)
                    return false;
                else
                {
                    for(const Vector *lhs=qlist.head,*rhs=sub->head;lhs;lhs=lhs->next,rhs=rhs->next)
                    {
                        assert(0!=lhs);
                        assert(0!=rhs);
                        if(*lhs!=*rhs) return false;
                    }
                    return true;
                }
            }

            bool Family:: hasSameSpanThan(const Family &sub)
            {
                return isIdenticalTo(sub) || isAnalogousTo(sub);
            }


            void Family:: generate(Random::Bits &ran, const size_t dim, size_t bits)
            {
                if(bits<=1) bits=1;
                assert(dim<=dimensions);
                reset();
                CxxArray<Integer,Memory::Dyadic> v(dimensions);
                while(qlist.size<dim)
                {
                    for(size_t i=dimensions;i>0;--i) v[i] = Integer(ran,bits);
                    if( welcomes(v) ) increase();
                }

            }

            const Apex::Natural & Family:: weight() const noexcept
            {
                return norm2.n;
            }



            std::ostream & operator<<(std::ostream &os, const Family &f)
            {

                os << "  {<" << Metrics::QualityText(f.quality)  << "> @weight=" << f.weight();
                if( f->size> 0)
                {
                    os << std::endl;
                    unsigned i = 1;
                    for(const Vector *v=f->head;v;v=v->next,++i)
                    {
                        os << "    " << "e" << i << "=" << *v << std::endl;
                    }
                }
                os << "  }";
                return os;
            }

            const Vector * Family:: increase()
            {
                assert(0!=qwork);
                assert(qwork->ncof>0);
                assert(qwork->nrm2>0);
                assert(qlist.size<dimensions);

                {
                    Apex::Integer _ = norm2 + qwork->nrm2;
                    _.xch(norm2);
                }

                qlist.pushTail(qwork);
                qwork = 0;
                Coerce(quality) = getQuality(qlist.size);
                {
                    Vector * const curr = qlist.tail;
                LOOP:
                    if(0!=curr->prev)
                        switch( Vector::Compare(*(curr->prev),*curr) )
                        {
                            case __Zero__: throw Specific::Exception("Ortho::Family", "unexpected multiple vectors!!");
                            case Negative: break;;
                            case Positive: qlist.towardsHead(curr); goto LOOP;
                        }
                    return curr;
                }
            }

            const char * const Family:: VectorCoefficient = "Vector Coefficient";


            void Family:: fetch(Writable<Integer> &target, const Int2Type<0> &) const
            {
                fetchInteger(target);
            }

            void Family:: fetch(Vector &target, const Int2Type<1> &) const
            {
                fetchVector(target);
            }

            void  Family:: fetchInteger(Writable<Integer> &target) const
            {
                assert(0!=qwork);
                assert(target.size()==dimensions);
                target.ld(*qwork);
            }

            void Family:: fetchVector(Vector &target) const
            {
                assert(0!=qwork);
                assert(target.size()==dimensions);
                target.ld(*qwork);
            }


        }
    }
}
