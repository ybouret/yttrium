//! \file

#ifndef Y_Chemical_ClusterContent_Included
#define Y_Chemical_ClusterContent_Included 1

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
        class ClusterContent : public Quantized, public Counted, public Fragment
        {
        public:
            typedef ArcPtr<const ClusterContent> Pointer;
            typedef ArcPtr<ClusterContent>       MutablePointer;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterContent(Equilibrium &first); //!< setup
            virtual ~ClusterContent() noexcept;          //!< cleanup
            Y_OSTREAM_PROTO(ClusterContent);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void attach(Equilibrium    &);                       //!< attach new equilibrium and its species
            void fusion(ClusterContent &)              noexcept; //!< fusion and update fragment
            bool accepts(const Equilibrium &)    const noexcept;
            bool accepts(const ClusterContent &) const noexcept;

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            SubEList equilibria;   //!< list of equilibria
            SubSList species;      //!< list of shared species species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterContent);
        };


    }

}

#endif

