
//! \file

#ifndef Y_Chemical_Prospect_Included
#define Y_Chemical_Prospect_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Prospect
        {
        public:
            typedef CxxSeries<Prospect,Memory::Dyadic> Series;

            ~Prospect() noexcept;

            Prospect(const Equilibrium &_eq,
                     const xreal_t      _xi,
                     const XReadable   &_cc,
                     const XReadable   &_dd) noexcept;

            Prospect(const Prospect &) noexcept;

            friend std::ostream & operator<<(std::ostream &os, const Prospect &pro)
            {
                os << pro.eq;
                return os;
            }

            static int Compare(const Prospect &lhs,
                               const Prospect &rhs) noexcept;

            const Equilibrium &eq; //!< used equilibirum
            const xreal_t      xi; //!< its solving extent
            const xreal_t      ax; //!< |xi|
            const XReadable   &cc; //!< its solving phase space
            const XReadable   &dd; //!< derivative
            const xreal_t      sl; //!< slope, initially 0
            
        private:
            Y_DISABLE_ASSIGN(Prospect);
        };
    }

}


#endif

