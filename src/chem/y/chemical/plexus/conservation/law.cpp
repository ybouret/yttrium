
#include "y/chemical/plexus/conservation/law.hpp"
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
            Actors(NameAsConcentration),
            GraphViz::Vizible(),
            denom(0),
            alpha(actors.size,denom),
            xproj(actors.size,actors.size),
            place(0),
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


                for(size_t i=dim;i>0;--i)
                {
                    for(size_t j=dim;j>0;--j)
                    {
                        Coerce(xproj[i][j]) = -(alpha[i] * alpha[j]);
                    }
                    Coerce(xproj[i][i]) += denom;
                }
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

            void  Law:: project(XAdd &xadd, XWritable &target, const XReadable  &source, const Level level) const
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

            

            std::ostream & operator<<(std::ostream &os, const Law &law)
            {
                const Actors &ac = law;
                return os << "d_(" << ac << ")";
            }


            //const char * const Law:: Scheme = "accent8";
            const char * const Law:: Scheme = "paired10";

            void  Law:: viz(OutputStream &fp) const
            {
                const Actors & self = *this;
                const String   uuid = self->html();
                const String   c_id = GraphViz::Vizible::Color(Scheme,place);
                const char * const color = c_id.c_str();
                Node(fp,this) << '[';
                fp << "label= <" << uuid << ">";
                fp << ",shape=box,style=bold,color=" << color << ",fontcolor=" << color;
                Endl(fp << ']');
                for(const Actor *a=self->head;a;a=a->next)
                {
                    const GraphViz::Vizible &source = a->sp;
                    const GraphViz::Vizible &target = *this;

                    Arrow(fp, &source, &target);
                    fp << "[style=dashed,arrowhead=empty,color=" << color << "]";
                    Endl(fp);
                }

            }


        }

    }
}
