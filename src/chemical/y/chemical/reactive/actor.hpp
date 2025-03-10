
//! \file


#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1


#include "y/chemical/species.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Actor : public Entity
        {
        public:
            enum Involvement
            {
                AsComponentOnly,
                AsConcentration
            };

            static const char * const CallSign; //!< "Actor"

            explicit Actor(const unsigned,
                           const Species &,
                           const Involvement);
            virtual ~Actor() noexcept;

            const unsigned    nu; //!< coefficient
            const Species &   sp; //!< persistent species
            const Involvement in;
            Actor *           next;
            Actor *           prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actor);
        };
    }

}

#endif
