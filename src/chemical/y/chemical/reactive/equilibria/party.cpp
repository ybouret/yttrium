
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
            uuid.lj(os, components.name) << ':';

            return os;
        }



    }

}


