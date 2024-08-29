
#include "y/chemical/plexus/warden/trim.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Trim:: Trim(const Fund &fund) noexcept :
        limiting(fund.sbank),
        required(fund)
        {
        }

        Trim:: ~Trim() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Trim &self)
        {
            os << self.limiting << "/" << self.required;
            return os;
        }

        void Trim:: free() noexcept
        {
            limiting.free();
            required.free();
        }


        bool Trim:: operator()(const XReadable   &C,
                               const Level        L,
                               const Actors      &A,
                               const AddressBook &conserved)
        {
            free();
            try
            {
                for(const Actor *a=A->head;a;a=a->next)
                {
                    const Species &sp = a->sp; if(!conserved.has(sp)) continue;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa>=0)
                    {
                        // update limiting
                        limiting(cc/a->xn,sp);
                    }
                    else
                    {
                        // insert in required
                        required(-cc/a->xn,sp);
                        assert(required.sorted());
                    }
                }
                return required.size>0;
            }
            catch(...)
            {
                free();
                throw;
            }
        }
    }

}

