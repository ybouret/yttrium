
#include "y/chemical/reactive/conservation/law.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            const char * const Law:: CallSign = "Chemical::Conservation::Law";
            Law:: ~Law() noexcept
            {
            }

            real_t toReal(const xReal &x) { return real_t(x); }

            Law:: Law(const Actor::List &actors) :
            Actors(),
            denom(0),
            alpha(actors.size,denom),
            xproj(actors.size,actors.size),
            next(0),
            prev(0)
            {
                const size_t dim = actors.size;
                if(dim<2) throw Specific::Exception(CallSign,"not enough species");

                Actors &self = *this;
                apn     sum2 = 0;
                size_t  i    = 0;
                for(const Actor *actor=actors.head;actor;actor=actor->next)
                {
                    const unsigned nu = actor->nu;
                    self(nu,actor->sp);
                    Coerce(alpha[++i]) = actor->xn;
                    sum2 += nu*nu;
                }
                Coerce(denom) = sum2.cast<unsigned>("conservation law denominator");

                std::cerr << "name  = '" << self->name << "'" << std::endl;
                Core::Display(std::cerr << "alpha = ", &alpha[1], alpha.size(), toReal) << std::endl;

                for(size_t i=dim;i>0;--i)
                {
                    for(size_t j=dim;j>0;--j)
                    {
                        Coerce(xproj[i][j]) = -(alpha[i] * alpha[j]);
                    }
                    Coerce(xproj[i][i]) += denom;
                }
                std::cerr << "xproj =" << xproj << std::endl;
            }


            xReal Law:: excess(XAdd &xadd, const XReadable &C, const Level L) const
            {
                xadd.free();
                for(const Actor *a=(*this)->head;a;a=a->next)
                {
                    xadd << a->xn * a->sp(C,L);
                }
                const xReal temp = xadd.sum();
                const xReal zero;
                if(temp<zero)
                {
                    return (temp*temp)/denom;
                }
                else
                {
                    return zero;
                }
            }

            void  Law:: excess(XAdd &xadd, XWritable &target, const XReadable  &source, const Level level) const
            {
                const Actors &      self = *this;
                const Actor * const head = self->head;
                
                target.ld(source);
                size_t I=1;
                for(const Actor *i=head;i;i=i->next,++i)
                {
                    xadd.free();
                    {
                        const XReadable &proj = xproj[I];
                        size_t           J=1;
                        for(const Actor *j=head;j;j=j->next,++J)
                        {
                            xadd << j->sp(source,level) * proj[J];
                        }
                    }
                    i->sp(target,level) = xadd.sum();
                }
            }




        }

    }
}
