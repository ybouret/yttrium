
#include "y/chemical/plexus/equalizer/extents.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Extents:: Extents(const EqzBanks &banks) noexcept :
        reac(banks),
        prod(banks),
        best(banks.sb)
        {
        }

        Extents:: ~Extents() noexcept
        {
        }

        void Extents:: restart() noexcept {
            reac.restart();
            prod.restart();
            best.restart();
        }


        void Extents:: operator()(const Components &  E,
                                  const XReadable &   C,
                                  const Level         L,
                                  const AddressBook * const wanders)
        {
            try
            {
                best.restart();
                reac(E.reac,C,L,wanders);
                prod(E.prod,C,L,wanders);


            }
            catch(...)
            {
                restart();
                throw;
            }

        }
    }

}

