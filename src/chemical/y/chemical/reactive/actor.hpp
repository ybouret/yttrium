
//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actor : public Object
        {
        public:
            explicit Actor(const unsigned, const Species &) noexcept;
            Actor(const Actor &) noexcept;
            virtual ~Actor() noexcept;

            const Species  &sp;   //!< persistent species
            const unsigned  nu;   //!< stoichiometric coefficient
            const XReal     xnu;  //!< nu as xreal
            const unsigned  nu1;  //!< nu-1
            Actor          *next; //!< for list
            Actor          *prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);
        };

    }

}

#endif
