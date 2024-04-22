
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
            next(0),
            prev(0),
            cast(),
            uuid()
            {

                const size_t m = coeff.size();
                for(size_t j=1;j<=m;++j)
                {
                    const unsigned nu=coeff[j]; if(0==nu) continue;
                    cast.pushTail( new Actor(nu,table[j]) );
                }
                String s = cast.toString();
                Coerce(uuid).swapWith(s);
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
                os << "d_(" << law.uuid << ")";
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

            const char * const Law::Colors = "dark28";

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

#if 0
                for(const Actor *curr=cast.head,*next=curr->next;next;curr=next,next=next->next)
                {

                    Arrow(fp,&(curr->sp),&(next->sp)) << '[';
                    fp << "dir=both,arrowhead=dot,arrowtail=dot,color=" << c;
                    Endl(fp << ']');
                }
#endif

            }

        }

    }

}


