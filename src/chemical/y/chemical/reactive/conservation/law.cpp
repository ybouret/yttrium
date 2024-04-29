
#include "y/chemical/reactive/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: Law(const String             &label,
                      const size_t              iboth,
                      const Readable<unsigned> &coeff,
                      const SpSubSet           &table) :
            Entity(label,iboth),
            Proxy<const Actors>(),
            nrm2(0),
            zero(0),
            alpha(),
            beta(),
            cast(),
            uuid(),
            next(0),
            prev(0)
            {
                const size_t m = coeff.size();
                apn          d = 0;
                for(size_t j=1;j<=m;++j)
                {
                    const unsigned nu=coeff[j]; if(0==nu) continue;
                    cast.pushTail( new Actor(nu,table[j]) );
                    d += (nu*nu);
                }
                String s = cast.toString();
                Coerce(uuid).swapWith(s);
                Coerce(nrm2) = static_cast<double>( d.cast<uint64_t>("|Chemical Law|^2") );
#if 0
                for(size_t i=1;i<=m;++i)
                {
                    Writable<xreal_t> &beta_i = Coerce(beta)[i];
                    const unsigned     coeff_i = coeff[i];
                    for(size_t j=1;j<=m;++j)
                    {
                        if(i==j)
                            beta_i[i] = nrm2;
                        beta_i[j] -= xreal_t( coeff_i * coeff[j] );
                    }
                }
#endif
            }


            Law:: ~Law() noexcept
            {
            }

            Law::ConstInterface & Law::surrogate() const noexcept { return cast; }

            const String & Law:: key() const noexcept
            {
                return uuid;
            }

            std::ostream & operator<<(std::ostream &os, const Law &law)
            {
                os << "d_(" << law.uuid << ")" ;//<< double(law.nrm2);
                return os;
            }


            bool Law:: sharesSpeciesWith(const Law &law) const noexcept
            {
                for(const Actor *a=cast.head;a;a=a->next)
                {
                    if(law->contains(a->sp)) return true;
                }
                return false;
            }

            const char * const Law::Colors = "set28";

            void Law:: viz(OutputStream &fp) const
            {
                assert(cast.size>=2);
                const String c = Color(Colors,indx[SubLevel]);

                Node(fp,this) << '[';
                Label(fp,name);
                fp << ",color=" << c << ",fontcolor=" << c << ",shape=circle";
                Endl(fp << ']');
                for(const Actor *a=cast.head;a;a=a->next)
                {
                    Arrow(fp,this,&(a->sp)) << '[';
                    fp << "color=" << c << ",fontcolor=" << c << ",arrowhead=dot";
                    if(a->nu>1) {
                        fp(",label=\"%u\"",a->nu);
                    }
                    Endl(fp << ']');
                }

            }

            void Law:: makeAlgebraic(const SList &species)
            {
                assert(species.size>=2);
                assert(cast.maxIndex(AuxLevel)>=1);
                assert(cast.maxIndex(AuxLevel)<=species.size);
                const size_t n = species.size;

                //______________________________________________________________
                //
                // build Alpha
                //______________________________________________________________
                {
                    VecType     &Alpha = Coerce(alpha); 
                    Alpha.adjust(n,zero);
                    for(const Actor *a=cast.head;a;a=a->next)
                    {
                        const Species &sp = a->sp; assert(species.has(sp));
                        Alpha[sp.indx[AuxLevel]] = a->xnu;
                    }
                }

                //______________________________________________________________
                //
                // build Beta
                //______________________________________________________________
                MatType     &Beta = Coerce(beta); 
                Beta.make(n,n);
                for(size_t i=1;i<=n;++i)
                {
                    Writable<xreal_t> &beta_i  = Beta[i];
                    const xreal_t      alpha_i = alpha[i];
                    for(size_t j=1;j<=n;++j)
                    {
                        if(i==j)
                            beta_i[i] = nrm2;
                        beta_i[j] -= alpha_i * alpha[j];
                    }
                }


            }



            xreal_t Law:: required(Writable<xreal_t>       &dC,
                                   const Level            outgoing,
                                   const Readable<xreal_t> &C,
                                   const Level            incoming,
                                   XAdd                  &xadd) const
            {
                // initialize (outgoin) dC
                dC.ld(zero);
                xadd.free();

                // collect negative balance
                for(const Actor *a=cast.head;a;a=a->next)
                {
                    xadd << a->xnu * C[ a->sp.indx[incoming] ];
                }
                const xreal_t scale = xadd.sum();
                assert(xadd.isEmpty());

                // act accordingly
                if(scale<zero)
                {
                    for(const Actor *a=cast.head;a;a=a->next)
                    {
                        const xreal_t d = -(scale * a->xnu)/nrm2;
                        dC[ a->sp.indx[outgoing] ] = d;
                    }
                    return (scale*scale)/nrm2;
                }
                else
                {
                    return zero;
                }
            }

            xreal_t Law:: required(Writable<xreal_t>       &Caux,
                                   const Readable<xreal_t> &Ctop,
                                   XAdd                    &xadd) const
            {
                const size_t ns = alpha.size(); assert(Caux.size()>=ns);

                xadd.make(ns);

                // collect dot product
                for(const Actor *a=cast.head;a;a=a->next)
                {
                    xadd << a->xnu * Ctop[ a->sp.indx[TopLevel] ];
                }

                // compute scale
                const xreal_t scale = xadd.sum();
                assert(xadd.isEmpty());

                if(scale<zero)
                {
                    // bad: in place compute Caux = (beta*Ctop)/nrm2
                    for(size_t i=ns;i>0;--i)
                    {
                        {
                            const Readable<xreal_t> &beta_i = beta[i];
                            for(const Actor *a=cast.head;a;a=a->next)
                            {
                                const Species &sp = a->sp;
                                xadd << beta_i[ sp.indx[AuxLevel] ] * Ctop[ sp.indx[TopLevel] ];
                            }
                        }
                        Caux[i] = xadd.sum()/nrm2;
                    }
                    return (scale*scale)/nrm2;
                }
                else
                {
                    // law is fullfilled
                    return zero;
                }
            }

        }

    }

}


