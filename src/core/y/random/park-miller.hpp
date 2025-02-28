
// \file

#ifndef Y_Random_Park_Miller_Included
#define Y_Random_Park_Miller_Included 1

#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {

        //______________________________________________________________________
        //
        //
        //
        //! Park And Miller minimal PRNG
        //
        //
        //______________________________________________________________________
        class ParkMiller : public Bits
        {
        public:
            static const char * const CallSign;                       //!< "Park-Miller"
            explicit         ParkMiller(const int32_t seed) noexcept; //!< setup with seed
            explicit         ParkMiller()                   noexcept; //!< setup with system seed
            virtual         ~ParkMiller()                   noexcept; //!< cleanup
            virtual uint32_t next32()                       noexcept; //!< get next dword
            virtual const char * callSign()           const noexcept; //!< CallSign

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParkMiller);
            int32_t state;
        };
    }

}

#endif

