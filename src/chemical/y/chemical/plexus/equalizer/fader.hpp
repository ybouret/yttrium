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

        //! fader for actors
        class Fader : public Recyclable
        {
        public:
            explicit Fader(const Banks &banks) noexcept;
            virtual ~Fader() noexcept;
            Y_OSTREAM_PROTO(Fader);

            virtual void free() noexcept;

            //! build required/limiting for limited equilibria
            bool operator()(const XReadable   &C,
                            const Level       &L,
                            const Actors      &A,
                            const AddressBook &conserved);

            Boundary   limiting; //!< limiting extent/species from C>=0
            Boundaries required; //!< required extent/species from C<0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fader);
        };
    }

}

#endif

