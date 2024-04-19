
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Actors as list
        //
        //
        //______________________________________________________________________
        class Actors :   public CxxListOf<Actor>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors() noexcept; //!< setup empty
            virtual ~Actors() noexcept; //!< cleanup
            Actors(const Actors &);     //!< copy

            //! display using toString
            friend std::ostream & operator<<(std::ostream &os, const Actors &A);

            //! sum(charge)
            int charge() const noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString() const; //!< cat all actors' string


        private:
            Y_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif
