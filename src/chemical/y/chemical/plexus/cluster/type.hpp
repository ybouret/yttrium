

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
        
        //______________________________________________________________________
        //
        //
        //
        //! Content of a Cluster
        //
        //
        //______________________________________________________________________
        class ClusterType : public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterType() noexcept; //!< setup
            virtual ~ClusterType() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            void attach(Equilibrium &eq);             //!< attach new equilibrium and its species
            void fusion(ClusterType &other) noexcept; //!< fusion and update fragment

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            SubEList equilibria;   //!< list of equilibria
            SubSList species;      //!< list of shared species species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterType);
        };

        
    }

}

#endif

