
#include "y/chemical/reactive/equilibrium/fmt.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        EqFormatter::  EqFormatter() noexcept {}
        EqFormatter:: ~EqFormatter() noexcept {}

        void EqFormatter:: upgradeWith(const Equilibrium &eq) noexcept
        {
            updateWith(eq);
            modernizeWith(eq);
        }

        std::ostream & EqFormatter:: display(std::ostream &os, const Equilibrium &eq) const
        {
            pad(os << '<' << eq.name << '>',eq) << Equilibrium::Separator;
            print(os,eq)                        << Equilibrium::Separator;
            os << std::setw(12) << Coerce(eq).K(0);
            return os;
        }
    }

}


