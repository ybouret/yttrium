
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Fragment:: ~Fragment() noexcept
        {
        }

        Fragment:: Fragment() noexcept :
        self(), reac(), prod()
        {
        }

        Fragment:: Fragment(const Fragment &_) noexcept :
        self(_.self),
        reac(_.reac),
        prod(_.prod)
        {
        }

        Fragment & Fragment:: operator=(const Fragment & _) noexcept
        {
            self = _.self;
            reac = _.reac;
            prod = _.prod;
            return *this;
        }

        void Fragment:: enroll(const Components &eq) noexcept
        {
            self.enroll(eq);
            reac.enroll(eq.reac);
            prod.enroll(eq.prod);
        }

        std::ostream & Fragment:: display(std::ostream &os, const Components &eq) const
        {
            self.pad(os << eq.name, eq)           << Components::Separator << ' ';
            reac.pad(os << eq.reac.name, eq.reac) << ' ' << Components::Symbol << ' ';
            prod.pad(os << eq.prod.name, eq.prod) << Components::Separator;
            return os;
        }

        std::ostream & Fragment:: display(std::ostream &os, const Components &eq, const xreal_t K) const
        {
            display(os,eq);
            const real_t lK = K.log10();
            const String sK = Formatted::Get("10^(%.4g)",lK);
            os << std::setw(12) << sK.c_str();
            return os;
        }


    }

}
