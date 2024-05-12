
//! \file

#ifndef Y_Chemical_Landscape_Included
#define Y_Chemical_Landscape_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Landscape
        {
        public:
            Landscape(const BBank &, const SBank &) noexcept ;
            Landscape(const Landscape &);
            ~Landscape() noexcept;
            Y_OSTREAM_PROTO(Landscape);

            //! reset all limits
            void reset() noexcept;

            //! shape limits from components and concentrations
            void shape(const Components &components,
                       const XReadable  &C,
                       const Level       level);


            
            Limits capping;
            Limits missing;

        private:
            Y_DISABLE_ASSIGN(Landscape);
        };

    }

}

#endif

