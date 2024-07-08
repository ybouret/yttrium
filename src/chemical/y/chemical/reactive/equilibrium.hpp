
//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equilibrium : public Components
        {
        public:
            typedef ArkPtr<String,Equilibrium> Ptr;
            virtual ~Equilibrium() noexcept;

        protected:
            template <typename ID> inline
            explicit Equilibrium(const ID    &userName,
                                 const size_t topLevel) :
            Components(userName,topLevel),
            zero(0)
            {
            }

        public:
            xreal_t K(const xreal_t);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal_t getK(const xreal_t) = 0;
            
            const xreal_t zero;
        };

    }

}

#endif

