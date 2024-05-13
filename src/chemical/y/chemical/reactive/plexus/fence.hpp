
//! \file

#ifndef Y_Chemical_Fence_Included
#define Y_Chemical_Fence_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/plexus/limits.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Fence made of boundaries for a controller
        //
        //
        //______________________________________________________________________
        class Fence
        {
        public:
            enum Status
            {
                Running,
                BlockedByBoth
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Fence(const BBank &, const SBank &) noexcept ; //!< setup
            Fence(const Fence &);                          //!< copy
            ~Fence() noexcept;                             //!< cleanup
            Y_OSTREAM_PROTO(Fence);                        //!< display

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
            Status study(const Boundary * &how, XMLog &xml);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Limits capping; //!< capping by positive concentrations
            Limits missing; //!< missing by negative concentrations

        private:
            Y_DISABLE_ASSIGN(Fence);
        };

    }

}

#endif

