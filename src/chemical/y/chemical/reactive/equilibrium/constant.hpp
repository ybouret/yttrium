
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
            template <typename ID> inline
            explicit ConstantEquilibrium(const ID     &userName,
                                         const size_t  topLevel,
                                         const xreal_t theValue) :
            Equilibrium(userName,topLevel), value(theValue) {}

            virtual ~ConstantEquilibrium() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstantEquilibrium);
            virtual xreal_t getK(const xreal_t);
            const xreal_t value;
        };

    }
}

#endif

