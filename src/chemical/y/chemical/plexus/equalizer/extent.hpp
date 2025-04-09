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
        namespace Equalizer
        {
            //__________________________________________________________________
            //
            //
            //
            //! classified extent from one side of an equilibirum
            //
            //
            //__________________________________________________________________
            class Extent : public Restartable
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                static const char * const Limiting; //!< "limiting"
                static const char * const Required; //!< "required"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Extent(const Banks &banks) noexcept; //!< setup empty
                virtual ~Extent() noexcept;                   //!< cleanup
                Y_OSTREAM_PROTO(Extent);                      //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________


                virtual void restart()      noexcept; //! restart all
                bool         online() const noexcept; //! if any of the list is populated

                //! dispatch according to C signs
                void operator()(const Actors &      A,
                                const XReadable &   C,
                                const Level         L,
                                const AddressBook  &wanders);


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Boundary limiting; //!< from positive concentrations
                Cursors  required; //!< for negative concentrations

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Extent);
            };

        }
    }

}


#endif

