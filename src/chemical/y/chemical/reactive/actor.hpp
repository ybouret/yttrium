//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Actor : nu>0 + species
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
            typedef CxxListOf<Actor> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit Actor(const unsigned n,
                           const Species &s,
                           const size_t   i);
            virtual ~Actor() noexcept;         //!< cleanup
            Actor(const Actor &);              //!< copy
            Y_OSTREAM_PROTO(Actor);            //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator*() const noexcept; //!< *this for list

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned  nu;       //!< coefficient
            const Species  &sp;       //!< species
            const unsigned  n1;       //!< nu-1
            Actor *         next;     //!< for list
            Actor *         prev;     //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);
            void initialize();

        };

    }
}

#endif

