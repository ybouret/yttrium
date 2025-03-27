

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
            proj((*rule)->size,(*rule)->size),
            uuid(0),
            next(0),
            prev(0)
            {
                static const char msg[] = "projection coefficient";
                {
                    Actors      &self = *this; Actors::Exchange(self,*rule);
                    iMatrix     &P    = Coerce(proj);
                    const apn    a2   = norm2;
                    {
                        size_t i=1;
                        for(const Actor *I=self->head;I;I=I->next,++i)
                        {
                            const apn alpha_i = I->nu;
                            size_t j=1;
                            for(const Actor *J=self->head;J;J=J->next,++j)
                            {
                                const apn alpha_j = J->nu;
                                const apn aa = alpha_i * alpha_j;
                                if(i==j)
                                {
                                    switch( Sign::Of(a2,aa) )
                                    {
                                        case __Zero__: P[i][j] = 0; break;
                                        case Positive: { const apn delta = a2-aa; P[i][j] = delta.cast<int>(msg); } break;
                                        case Negative: { const apn delta = aa-a2; P[i][j] = delta.cast<int>(msg); } break;
                                    }
                                }
                                else
                                {
                                    P[i][j] = -aa.cast<int>(msg);
                                }
                            }
                        }
                    }
                }
                //std::cerr << "proj=" << proj << std::endl;
                latch();
            }


            void Law:: viz(OutputStream &fp, const String * const color) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                if(color) fp << ',' << *color;
                fp << ",shape=box,style=rounded";
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
                    xadd.insert( a->sp(C,L), a->nu );
                const xreal_t xs = xadd.sum();
                const xreal_t _0;
                if(xs<_0) return -xs; else return _0;
            }

            void Law:: project(XAdd      &       xadd,
                               XWritable &       Cp, const Level Lp,
                               const XReadable & C0, const Level L0) const
            {
                const Actors &self = *this;
                {
                    size_t               i   = 1;
                    const Readable<int> &P_i = proj[i];
                    for(const Actor *I=self->head;I;I=I->next,++i)
                    {
                        size_t j=1;
                        xadd.free();
                        for(const Actor *J=self->head;J;J=J->next,++J)
                        {
                            const int P_ij = P_i[j];
                            switch(P_ij)
                            {
                                case __Zero__: break;
                                case Positive: xadd.insert(J->sp(C0,L0), P_ij); break;
                                case Negative: xadd.insert(J->sp(C0,L0),-P_ij); break;
                            }
                        }
                        I->sp(Cp,Lp) = xadd.sum()/denom;
                    }
                }
            }

        }
    }

}

