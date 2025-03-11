
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
        //! Single Actor
        //
        //
        //______________________________________________________________________
        class Actor : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //! for components or concentration
            enum Involvement
            {
                AsComponentOnly, //!< for components
                AsConcentration  //!< for conservations,...
            };

            static const char * const CallSign; //!< "Actor"
            typedef CxxListOf<Actor>  List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit Actor(const unsigned,
                           const Species &,
                           const Involvement);
            Actor(const Actor &) noexcept; //!< copy with shared entity name
            virtual ~Actor()     noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned    nu;   //!< coefficient
            const Species &   sp;   //!< persistent species
            Actor *           next; //!< for list
            Actor *           prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);
        };
    }

}

#endif
