//! \file

#ifndef Y_Chemical_Mixed_Equilibrium_Included
#define Y_Chemical_Mixed_Equilibrium_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/woven/survey/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class MixedEquilibrium : public Equilibrium
        {
        public:

            static String MakeName(const EList               &eqs,
                                   const WOVEn::IntegerArray &cof);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
        };

    }
}

#endif

