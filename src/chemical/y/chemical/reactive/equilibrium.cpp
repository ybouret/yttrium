
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equilibrium:: ~Equilibrium() noexcept
        {
        }


        xreal_t Equilibrium:: K(const xreal_t t)
        {
            const xreal_t value = getK(t);
            if(value<=zero) throw Specific::Exception( name.c_str(), "negative K at t=%g", double(real_t(t)));
            return value;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            os << eq.name;
            return os;
        }

        Equilibrium::Comparator::  Comparator() noexcept {}
        Equilibrium::Comparator:: ~Comparator() noexcept {}


        SignType Equilibrium::Comparator:: operator()(const Equilibrium &lhs, const Equilibrium &rhs) const noexcept
        {
            return Sign::Of(lhs.indx[TopLevel],rhs.indx[TopLevel]);
        }


    }

}
