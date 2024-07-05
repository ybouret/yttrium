
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
            Y_OSTREAM_PROTO(Actors);    //!< display using toString

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString()                           const; //!< cat all actors' string
            bool   checked()                   const noexcept; //!< different species with positive nu
            int    charge()                    const noexcept; //!< sum(charge)
            bool   contains(const Species &)   const noexcept; //!< look for species
            bool   contains(const Actor   &)   const noexcept; //!< look for species+coeff
            size_t maxIndex(const Level level) const noexcept; //!< max indx[level]
            friend bool operator==(const Actors & , const Actors & ) noexcept; //!< check same content


        private:
            Y_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif
