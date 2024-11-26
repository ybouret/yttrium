//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/defs.hpp"


namespace Yttrium
{
    namespace Chemical
    {

       
        class Actor : public Entity
        {
        public:
            static const char * const CallSign;
            typedef CxxListOf<Actor> List;




            explicit Actor(const unsigned, const Species &);
            explicit Actor(const Species &);
            virtual ~Actor() noexcept;
            Actor(const Actor &);

            virtual const String & key() const noexcept;

            const unsigned nu; //!< stoichio>0
            const Species &sp; //!< species
            const xReal    xn; //!< xreal(nu)
            const unsigned n1; //!< nu-1
            Actor *        next;
            Actor *        prev;
            const String   name;

        private:
            Y_DISABLE_ASSIGN(Actor);
        };

    }

}

#endif

