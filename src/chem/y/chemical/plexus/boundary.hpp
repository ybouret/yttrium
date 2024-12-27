//! \file

#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/species.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Boundary
        {
        public:
            Boundary(const Species & _sp, const xReal _xx) noexcept;
            Boundary(const Boundary &)                     noexcept;
            ~Boundary()                                    noexcept;

            const String & key()       const noexcept;
            const xReal &  operator*() const noexcept;
            xReal       &  operator*()       noexcept;

            const Species &sp;

        private:
            xReal          xx;

            Y_DISABLE_ASSIGN(Boundary);
        };


    }

}

#endif

