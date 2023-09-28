//! \file


#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chem/species.hpp"
#include "y/chem/types.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actor : public Object
        {
        public:
            explicit Actor(const Species &which,
                           const unsigned coeff) noexcept;
            Actor(const Actor &) noexcept;
            virtual ~Actor() noexcept;
        
            const Species &sp;
            const unsigned nu; //!< nu > 0
            const unsigned n1; //!< n1=nu-1

            Actor *next;
            Actor *prev;

        private:
            Y_DISABLE_ASSIGN(Actor);    
        };

    }
}

#endif
