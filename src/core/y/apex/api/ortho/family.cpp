
#include "y/apex/api/ortho/family.hpp"
#include "y/system/exception.hpp"

 
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

            Family:: Family(const VCache &c) noexcept :
            Metrics(*c),
            Proxy<const Vector::List>(),
            quality( getQuality(0) ),
            qlist(),
            qwork(0),
            cache(c) 
            {
            }

            Family:: Family(const Family &_) :
            Metrics(_),
            Proxy<const Vector::List>(),
            quality(_.quality),
            qlist(),
            qwork(0),
            cache(_.cache)
            {
                for(const Vector *v=_->head;v;v=v->next)
                {
                    qlist.pushTail( cache->query(*v) );
                }
            }



            
            Y_PROXY_IMPL(Family,qlist)

#if 1
            void Family:: recreate(const Family &F)
            {
                assert(this != &F);
                reset();
                try {
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
#endif

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

            const Vector * Family:: increase()
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
