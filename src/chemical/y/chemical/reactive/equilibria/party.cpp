
#include "y/chemical/reactive/equilibria/party.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Party::  Party() noexcept : uuid(), reac(), prod() {}

        Party:: ~Party() noexcept {}


        void Party:: update(const Components &components) noexcept
        {
            Coerce(uuid).enroll(components);
            Coerce(reac).enroll(components.reac);
            Coerce(prod).enroll(components.prod);

        }

        std::ostream & Party:: display(std::ostream &os, const Components &components) const
        {
            uuid.lj(os, components.name) << " : ";
            reac.rj(os, components.reac.name);
            os << ' ' << Equilibrium::Mark << ' ';
            prod.lj(os, components.prod.name);// << " : ";
            
            return os;
        }


        std::ostream & Party:: display(std::ostream &os, const Components &eq, const XReadable &K) const
        {
            const xreal_t k   = K[eq.indx[TopLevel]];
            const real_t  l10 = k.log10();
            return display(os,eq) << " : 10^(" << l10 << ")";
        }



    }

}


