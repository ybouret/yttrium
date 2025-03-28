
#include "y/chemical/plexus/equalizer/extents.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Extents:: Extents(const EqzBanks &banks) noexcept :
        reac(banks),
        prod(banks)
        {
        }

        Extents:: ~Extents() noexcept
        {
        }

        void Extents:: restart() noexcept { reac.restart(); prod.restart(); }


        void Extents:: operator()(const Components &  E,
                                  const XReadable &   C,
                                  const Level         L,
                                  const AddressBook * const wanders)
        {
            reac(E.reac,C,L,wanders);
            prod(E.prod,C,L,wanders);
        }

    }

}

