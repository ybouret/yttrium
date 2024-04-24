
//! \file

#ifndef Y_Chemical_Equilibrium_Constant_Included
#define Y_Chemical_Equilibrium_Constant_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium with fixed constant
        //
        //
        //______________________________________________________________________
        class ConstantEquilibrium : public Equilibrium
        {
        public:

            //! setup
            template <typename UID> inline
            explicit ConstantEquilibrium(const UID     &uid,
                                         const size_t   topLevel,
                                         const xreal_t  value) :
            Equilibrium(uid,topLevel),
            K_(value)
            {
            }

            //! cleanup
            virtual ~ConstantEquilibrium() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstantEquilibrium);
            const xreal_t K_;
            virtual xreal_t getK(real_t) noexcept;
        };

    }

}

#endif

