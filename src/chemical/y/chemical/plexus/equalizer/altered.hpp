
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
                             const XReadable   & _cc,
                             const xreal_t       _gg) noexcept :
            eq(_eq),
            cc(_cc),
            gg(_gg)
            {
            }

            Altered(const Altered &_) noexcept :
            eq(_.eq),
            cc(_.cc),
            gg(_.gg)
            {
            }

            static int Compare(const Altered &lhs, const Altered &rhs) noexcept
            {
                return Comparison:: Decreasing(lhs.gg, rhs.gg);
            }

            ~Altered() noexcept {}

            const Equilibrium & eq;
            const XReadable   & cc;
            const xreal_t       gg;

        private:
            Y_DISABLE_ASSIGN(Altered);
        };
    }

}

#endif

