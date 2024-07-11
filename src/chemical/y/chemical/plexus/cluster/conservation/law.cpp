
#include "y/chemical/plexus/cluster/conservation/law.hpp"
#include "y/system/exception.hpp"

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
            next(0),
            prev(0)
            {
                assert( species.size == coef.size() );
                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species  &s = **sn;
                    const size_t    j = s.indx[SubLevel];
                    const unsigned  n = coef[j];
                    if(n>0) (*this)(n,s);
                }
                if((*this)->size<=1) throw Specific::Exception("Conservation::Law", "not enough species!!");
            }



            std::ostream & operator<<(std::ostream &os, const Law &law)
            {
                const Actors &ac = law;
                os << "d_(" << ac << ")";
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


        }

    }

}
