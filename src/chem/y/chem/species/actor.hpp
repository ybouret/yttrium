//! \file


#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chem/species.hpp"
#include "y/chem/type/defs.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! an actor is a species with a positive coefficient
        //
        //
        //______________________________________________________________________
        class Actor : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actor(const Species &which,
                           const unsigned coeff) noexcept; //!< setup
            Actor(const Actor &)                 noexcept; //!< copy
            virtual ~Actor()                     noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString() const; //!< convert to string

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Species &sp;   //!< persistent species
            const unsigned nu;   //!< nu > 0
            const unsigned n1;   //!< n1=nu-1
            Actor *        next; //!< for list
            Actor *        prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);    
        };

    }
}

#endif
