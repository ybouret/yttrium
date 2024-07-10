
#include "y/chemical/plexus/cluster/conservation/law.hpp"

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
            }



            std::ostream & operator<<(std::ostream &os, const Law &law)
            {
                const Actors &ac = law;
                os << "d_(" << ac << ")";
                return os;
            }
        }

    }

}
