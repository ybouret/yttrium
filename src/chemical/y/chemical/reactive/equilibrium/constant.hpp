
//! \file

#ifndef Y_Chemical_Equilibrium_Constant_Included
#define Y_Chemical_Equilibrium_Constant_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class ConstantEquilibrium : public Equilibrium
        {
        public:
            template <typename UID> inline
            explicit ConstantEquilibrium(const UID   &uid,
                                         const size_t topLevel,
                                         const XReal  value) :
            Equilibrium(uid,topLevel),
            K_(value)
            {
            }

            virtual ~ConstantEquilibrium() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstantEquilibrium);
            const XReal K_;
            virtual XReal getK(Real) noexcept;
        };

    }

}

#endif

