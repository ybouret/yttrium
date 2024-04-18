//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Equilibrium : public Entity, public Components
        {
        public:
            typedef ArkPtr<String,Equilibrium> Handle;

            virtual ~Equilibrium() noexcept;
            XReal    K(Real t);

        protected:
            template <typename UID> inline
            explicit Equilibrium(const UID   &uid,
                                 const size_t topLevel) :
            Entity(uid,topLevel), Components()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual XReal getK(Real t) = 0;
        };

    }

}

#endif


