

//! \file


#ifndef Y_Chemical_ClusterType_Included
#define Y_Chemical_ClusterType_Included 1

#include "y/chemical/reactive/fragment.hpp"
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/type/meta-list.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class ClusterType : public Fragment
        {
        public:
            explicit ClusterType() noexcept;
            virtual ~ClusterType() noexcept;

            void attach(Equilibrium &eq);             //!< attache new equilibrium and its species
            void fusion(ClusterType &other) noexcept; //!< fusion and update fragment

            SubEList equilibria;   //!< list of equilibria
            SubSList species;      //!< list of shared species species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterType);
        };

        
    }

}

#endif

