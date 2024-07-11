
#include "y/chemical/plexus/cluster/conservation/law.hpp"
#include "y/system/exception.hpp"

#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Law:: ~Law() noexcept
            {
            }

            Law:: Law(const SList              &species,
                      const Readable<unsigned> &coef) :
            Actors(),
            xden(0),
            proj(),
            keep(),
            next(0),
            prev(0)
            {
                assert( species.size == coef.size() );

                apn sq;
                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species  &s = **sn;
                    const size_t    j = s.indx[SubLevel];
                    const unsigned  n = coef[j];
                    if(n>0)
                    {
                        (*this)(n,s);
                        sq += n*n;
                    }
                }
                if((*this)->size<=1) throw Specific::Exception("Conservation::Law", "not enough species!!");
                Coerce(xden) = sq.cast<unsigned>("|law|^2");

            }



            std::ostream & operator<<(std::ostream &os, const Law &law)
            {
                const Actors &ac = law;
                os << "d_(" << ac << ")";// << law.keep;
                return os;
            }

            void Law:: viz(OutputStream &fp, const String &color) const
            {
                Node(fp,this) << '[';
                Label(fp,name);
                fp << ",color=" << color << ",fontcolor=" << color;
                fp << ",shape=note";
                fp << ']';
                Endl(fp);
                for(const Actor *a=(*this)->head;a;a=a->next)
                    a->viz(fp, *this, color, Iterating::Reverse);
            }


            bool Law:: linkedTo(const Species &sp) const noexcept
            {
                return hired(sp);
            }

            bool Law:: linkedTo(const Law &law) const noexcept
            {
                for(const Actor *a=law->head;a;a=a->next)
                {
                    if(hired(a->sp)) return true;
                }
                return false;
            }

            xreal_t Law:: excess(const XReadable &C, const Level level, XAdd &xadd) const
            {
                xadd.free();
                for(const Actor *a=(*this)->head;a;a=a->next)
                {
                    const xreal_t p = C[a->sp.indx[level]] * a->xn;
                    xadd << p;
                }
                return xadd.sum();
            }

            bool Law :: broken(xreal_t &         score,
                               XWritable &       Cout,
                               const Level       Lout,
                               const XReadable & Cinp,
                               const Level       Linp,
                               XAdd             &xadd) const
            {
                const xreal_t zero;

                // evaluate negative excess
                xadd.free();
                const Actor * const head = (*this)->head;
                for(const Actor *a=head;a;a=a->next)
                {
                    const xreal_t p = Cinp[a->sp.indx[Linp]] * a->xn;
                    xadd << p;
                }
                const xreal_t xs = xadd.sum();  if(xs>=zero) return false;
                score = (xs*xs)/xden;
                Cout.ld(zero);

                // projection for actors
                for(const Actor *i=head;i;i=i->next)
                {
                    const XReadable &p = proj[i->sp.indx[AuxLevel]];
                    xadd.free();
                    for(const Actor *j=head;j;j=j->next)
                    {
                        xadd << p[j->sp.indx[AuxLevel]] * Cinp[j->sp.indx[Linp]];
                    }
                    Cout[i->sp.indx[Lout]] = xadd.sum() / xden;
                }

                // transfer species to keep unchanged
                for(const SNode *sn=keep.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    Cout[ sp.indx[Lout] ] = Cinp[ sp.indx[Linp] ];
                }


                return true;
            }


#if 0
            xreal_t Law:: inject(const xreal_t factor, XWritable &deltaC, const Level level, XAdd &xadd) const
            {
                xadd.free();
                for(const Actor *a=(*this)->head;a;a=a->next)
                {
                    const xreal_t p = (factor * a->xn)/xden;
                    xadd << p.abs();
                    deltaC[a->sp.indx[level]] = p;
                }
                return xadd.sum();
            }
#endif

        }

    }

}
