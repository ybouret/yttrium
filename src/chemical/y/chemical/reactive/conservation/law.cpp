
#include "y/chemical/reactive/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: Law(const Readable<unsigned> &coeff,
                      const SpSubSet           &table) :
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


        }

    }

}


