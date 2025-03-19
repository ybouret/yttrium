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
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef ArcPtr<const ClusterContent> Pointer;         //!< for cluster
            typedef ArcPtr<ClusterContent>       MutablePointer;  //!< for builder

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterContent(Equilibrium &first); //!< setup
            virtual ~ClusterContent() noexcept;          //!< cleanup
            Y_OSTREAM_PROTO(ClusterContent);             //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void attach(Equilibrium    &);                       //!< attach new equilibrium and its species
            void fusion(ClusterContent &)              noexcept; //!< fusion and update fragment
            bool accepts(const Equilibrium &)    const noexcept; //!< if has shared species
            bool accepts(const ClusterContent &) const noexcept; //!< if has shared species

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            SubEList equilibria;   //!< list of equilibria, initial then expanded
            SubSList species;      //!< list of shared species among equilibria
            Assembly sformat;      //!< format for species
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterContent);
            void updateFmt() noexcept;
        };


    }

}

#endif

