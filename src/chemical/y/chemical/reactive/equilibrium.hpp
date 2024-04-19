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
            static const char Separator = ':';
            typedef Small::BareLightList<Equilibrium> BareList;
            
            typedef ArkPtr<String,Equilibrium> Handle;
            typedef SuffixSet<String,Handle>   Set;

 
            std::ostream & showEquilibrium(std::ostream &os,
                                           const size_t nmax=0,
                                           const size_t rmax=0,
                                           const size_t pmax=0) const;

            XReal    K(Real t);

            virtual ~Equilibrium() noexcept;
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


