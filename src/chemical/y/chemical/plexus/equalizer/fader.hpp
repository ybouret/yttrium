//! \file

#ifndef Y_Chemical_Fader_Included
#define Y_Chemical_Fader_Included 1

#include "y/chemical/plexus/equalizer/boundaries.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Fader for actors: smallest limiting and all required
        //
        //
        //______________________________________________________________________
        class Fader : public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fader(const Banks &banks) noexcept; //!< setup
            virtual ~Fader()                   noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Fader);                      //!< display

            //__________________________________________________________________
            //
            //
            // [Recyclable]
            //
            //__________________________________________________________________
            virtual void free() noexcept; //!< clear all

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build required/limiting for limited equilibria, return required.size>0
            bool operator()(const XReadable   &C,
                            const Level       &L,
                            const Actors      &A,
                            const AddressBook &conserved);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Boundary   limiting; //!< limiting extent/species from C>=0
            Boundaries required; //!< required extent/species from C<0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };
    }

}

#endif

