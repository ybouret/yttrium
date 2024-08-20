//! \file

#ifndef Y_Chemical_Faders_Included
#define Y_Chemical_Faders_Included 1

#include "y/chemical/plexus/equalizer/fader.hpp"
#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Faders for Components
        //
        //
        //______________________________________________________________________
        class Faders : public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned BALANCED = 0x00;                      //!< alias
            static const unsigned BAD_REAC = 0x01;                      //!< alias
            static const unsigned BAD_PROD = 0x02;                      //!< alias
            static const unsigned BAD_BOTH = BAD_REAC | BAD_PROD;       //!< alias
            typedef CxxArray<Faders,XMemory> Array;                     //!< alias
            static const char * TextFlag(const unsigned flag) noexcept; //!< helper

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Faders(const Banks &) noexcept; //!< setup
            virtual ~Faders()              noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Faders);                 //!< display

            //__________________________________________________________________
            //
            //
            // [Recyclable]
            //
            //__________________________________________________________________
            virtual void free() noexcept; //!< free reac and prod


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! evaluate and return status
            unsigned operator()(const XReadable   &C,
                                const Level       &L,
                                const Components  &E,
                                const AddressBook &conserved);
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Fader reac; //!< limiting/required on reac size
            Fader prod; //!< limiting/required on prod size

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Faders);
        };
    }

}

#endif

