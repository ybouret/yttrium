

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: ~Law() noexcept
            {
            }

            static unsigned ActorsNorm2(const Actors &actors)
            {
                apn n2 = 0;
                for(const Actor *a=actors->head;a;a=a->next)
                {
                    n2 += Squared(a->nu);
                }

                return n2.cast<unsigned>(actors.name->c_str());
            }

            Law:: Law(Rule * const rule) :
            Actors(rule->in),
            norm2(ActorsNorm2(*rule)),
            denom(norm2),
            auxId(0),
            subId(0),
            next(0),
            prev(0)
            {
                Actors &self = *this;
                Actors::Exchange(self,*rule);
                latch();
            }


            void Law:: viz(OutputStream &fp, const String * const color) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                if(color) fp << ',' << *color;
                fp << ",shape=box,style=diagonals";
                Endl(fp << ']');
                for(const Actor *a=(*this)->head;a;a=a->next)
                {
                    Arrow(fp,this,&a->sp) << '[';
                    fp << "arrowhead=odot";
                    if(color) fp << ',' << *color;
                    Endl(fp << ']');
                }
            }

            xreal_t Law:: excess(XAdd &xadd, const XReadable &C, const Level L) const
            {
                xadd.free();
                for(const Actor *a=(*this)->head;a;a=a->next)
                    xadd.insert( a->sp(C,L) * a->xn );
                const xreal_t xs = xadd.sum();
                const xreal_t _0;
                if(xs<_0) return -xs; else return _0;
            }

            
            bool Law:: hasCommonActorWith(const Law &other) const noexcept
            {
                for(const Actor *a=other->head;a;a=a->next)
                {
                    if( has(a->sp) ) return true;
                }
                return false;
            }


        }
    }

}

