
#include "y/chemical/plexus/equalizer/fader.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fader:: Fader(const Banks &banks) noexcept:
        limiting(banks.s),
        required(banks)
        {
        }

        Fader:: ~Fader() noexcept {}

        void Fader:: free() noexcept
        {
            limiting.empty();
            required.free();
        }

        std::ostream & operator<<(std::ostream &os, const Fader &f)
        {
            os << "lim="  << f.limiting;
            os << "/req=" << f.required;
            return os;
        }

        bool Fader:: operator()(const XReadable  &C,
                                const Level      &L,
                                const Actors     &A,
                                const AddressBook &conserved)
        {
            assert(0==limiting.size);
            assert(0==required.size);

            // dispatch required, build limiting
            for(const Actor *a=A->head;a;a=a->next)
            {
                const Species &sp = a->sp; if( !conserved.has(sp) ) continue;
                const xreal_t  cc = C[ sp.indx[L] ];
                if(cc.mantissa<0)
                {
                    required((-cc)/a->xn,sp);
                    assert(required.sorted());
                }
                else
                {
                    limiting(cc/a->xn,sp);
                }
            }

            // return if bad
            return required.size>0;
        }

    }

}
