//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Actor = nu * species
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
            explicit Actor(const unsigned, const Species &) noexcept; //!< setup nu>0
            Actor(const Actor &) noexcept;                            //!< copy;
            virtual ~Actor()     noexcept;                            //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String asFirst() const; //!< convert to string
            String asExtra() const; //!< convert to string

            int charge() const noexcept; //!< nu * sp.z

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Species  &sp;   //!< persistent species
            const unsigned  nu;   //!< stoichiometric coefficient
            const xreal_t   xnu;  //!< nu as xreal
            const unsigned  nu1;  //!< nu-1
            Actor          *next; //!< for list
            Actor          *prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);
            void fmt(String &) const;
        };



    }

}

#endif
