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
        //______________________________________________________________________
        //
        //
        //
        //! classified extent from one side of an equilibirum
        //
        //
        //______________________________________________________________________
        class Extent : public Restartable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static const char * const Limiting; //!< "limiting"
            static const char * const Required; //!< "required"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Extent(const EqzBanks &banks) noexcept; //!< setup empty
            virtual ~Extent() noexcept;                      //!< cleanup
            Y_OSTREAM_PROTO(Extent);                         //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! restart all
            virtual void restart() noexcept;

            //! dispatch according to C signs
            void operator()(const Actors &      A,
                            const XReadable &   C,
                            const Level         L,
                            const AddressBook * const wanders);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Boundary limiting; //!< from positive concentrations
            Cursors  required; //!< for negative concentrations

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extent);
        };


    }

}


#endif

