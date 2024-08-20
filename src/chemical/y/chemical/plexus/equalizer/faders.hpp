//! \file

#ifndef Y_Chemical_Faders_Included
#define Y_Chemical_Faders_Included 1

#include "y/chemical/plexus/equalizer/fader.hpp"
#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Faders : public Recyclable
        {
        public:
            static const unsigned BALANCED = 0x00;
            static const unsigned BAD_REAC = 0x01;
            static const unsigned BAD_PROD = 0x02;
            static const unsigned BAD_BOTH = BAD_REAC | BAD_PROD;
            typedef CxxArray<Faders,XMemory> Array;

            static const char * TextFlag(const unsigned flag) noexcept;


            explicit Faders(const Banks &) noexcept;
            virtual ~Faders() noexcept;
            Y_OSTREAM_PROTO(Faders);

            virtual void free() noexcept;

            unsigned operator()(const XReadable   &C,
                                const Level       &L,
                                const Components  &E,
                                const AddressBook &conserved);
            
            Fader reac;
            Fader prod;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Faders);
        };
    }

}

#endif

