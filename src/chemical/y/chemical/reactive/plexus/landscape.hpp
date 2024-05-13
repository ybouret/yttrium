
//! \file

#ifndef Y_Chemical_Landscape_Included
#define Y_Chemical_Landscape_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Landscape of boundaries for a controller
        //
        //
        //______________________________________________________________________
        class Landscape
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Landscape(const BBank &, const SBank &) noexcept ; //!< setup
            Landscape(const Landscape &);                      //!< copy
            ~Landscape() noexcept;                             //!< cleanup
            Y_OSTREAM_PROTO(Landscape);                        //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! reset all limits
            void reset() noexcept;

            //! shape limits from CONSERVED components and concentrations
            void shape(const Components &components,
                       const XReadable  &C,
                       const Level       level);

            //! todo
            void study();
            

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Limits capping; //!< capping by positive concentrations
            Limits missing; //!< missing by negative concentrations

        private:
            Y_DISABLE_ASSIGN(Landscape);
        };

    }

}

#endif

