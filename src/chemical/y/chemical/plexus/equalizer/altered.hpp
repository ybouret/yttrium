
//! \file

#ifndef Y_Chemical_Altered_Included
#define Y_Chemical_Altered_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Altered
        {
        public:
            typedef CxxSeries<Altered,XMemory> Series;

            explicit Altered(const Equilibrium & _eq,
                             const xreal_t       _xi,
                             const XReadable   & _cc,
                             const xreal_t       _gg,
                             const XReadable   & _dc) noexcept :
            eq(_eq),
            xi(_xi),
            cc(_cc),
            gg(_gg),
            dc(_dc)
            {
            }

            Altered(const Altered &_) noexcept :
            eq(_.eq),
            xi(_.xi),
            cc(_.cc),
            gg(_.gg),
            dc(_.dc)
            {
            }

            static int Compare(const Altered &lhs, const Altered &rhs) noexcept
            {
                return Comparison:: Decreasing(lhs.gg, rhs.gg);
            }

            ~Altered() noexcept {}

            const Equilibrium & eq;
            const xreal_t       xi;
            const XReadable   & cc;
            const xreal_t       gg;
            const XReadable   & dc;
        private:
            Y_DISABLE_ASSIGN(Altered);
        };
    }

}

#endif

