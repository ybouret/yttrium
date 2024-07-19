//! \file

#ifndef Y_Chemical_Eqs_Deps_Included
#define Y_Chemical_Eqs_Deps_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/ranked.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class LinearlyIndependent
        {
        public:
            static const char * const CallSign;
            
            typedef Small::SoloLightList<const Equilibrium>      ESolo;
            typedef Small::Ranked<ESolo,Equilibrium::Comparator> ERank;

            explicit LinearlyIndependent(const size_t capacity);
            virtual ~LinearlyIndependent() noexcept;

            void start(const Equilibrium &eq);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearlyIndependent);
            ERank list;

            bool grow(const Equilibrium &eq);

        };

    }

}


#endif
