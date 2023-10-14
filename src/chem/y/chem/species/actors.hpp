//! \file


#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chem/species/actor.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Actors
        //
        //
        //______________________________________________________________________
        class Actors : public CxxListOf<Actor>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Actor> BaseType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors() noexcept; //!< setup empty
            virtual ~Actors() noexcept; //!< cleanup
            Actors(const Actors &);     //!< copy
            Y_OSTREAM_PROTO(Actors);    //!< display using string
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString()                    const; //!< to string
            bool   has(const Species &) const noexcept; //!< check 

        private:
            Y_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif
