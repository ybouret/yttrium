
//! \file

#ifndef Y_Chemical_Equilibria_Fragment_Included
#define Y_Chemical_Equilibria_Fragment_Included 1

#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Fragment : public EList
        {
        public:
            explicit Fragment() noexcept;
            virtual ~Fragment() noexcept;
            Fragment(const Fragment &);

            const SList species;
        private:
            Y_DISABLE_ASSIGN(Fragment);
        };
    }

}

#endif

