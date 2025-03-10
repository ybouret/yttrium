
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
            typedef CxxListOf<Actor>  List;

            explicit Actor(const unsigned,
                           const Species &,
                           const Involvement);
            Actor(const Actor &) noexcept; //!< 'shared' copy
            virtual ~Actor()     noexcept;



            const unsigned    nu; //!< coefficient
            const Species &   sp; //!< persistent species
            Actor *           next;
            Actor *           prev;

        private:
            Y_DISABLE_ASSIGN(Actor);
        };
    }

}

#endif
