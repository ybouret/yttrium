
//! \file


#ifndef Y_Chemical_Equilibrium_Raw_Included
#define Y_Chemical_Equilibrium_Raw_Included 1


#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class RawEquilibrium : public Equilibrium
        {
        public:
            explicit RawEquilibrium(Library &,  XNode * const);
            virtual ~RawEquilibrium() noexcept;

            const AutoPtr<const String> Kdata; //!< raw string from xnode

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RawEquilibrium);
        };
    }

}

#endif

