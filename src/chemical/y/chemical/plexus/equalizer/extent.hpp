//! \file


#ifndef Y_Chemical_Extent_Included
#define Y_Chemical_Extent_Included 1

#include "y/chemical/plexus/equalizer/cursors.hpp"
#include "y/chemical/plexus/equalizer/boundary.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Extent : public Restartable
        {
        public:
            static const char * const Limiting;
            static const char * const Required;

            explicit Extent(const EqzBanks &banks) noexcept;
            virtual ~Extent() noexcept;
            Y_OSTREAM_PROTO(Extent);

            //! restart all
            virtual void restart() noexcept;

            //! dispatch according to C signs
            void operator()(const Actors &      A,
                            const XReadable &   C,
                            const Level         L,
                            const AddressBook * const wanders);


            Boundary limiting;
            Cursors  required;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extent);
        };


    }

}


#endif

