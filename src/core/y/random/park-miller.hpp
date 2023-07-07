
// \file

#ifndef Y_Random_Park_Miller_Included
#define Y_Random_Park_Miller_Included 1

#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {

        class ParkMiller : public Bits
        {
        public:
            explicit ParkMiller(const int32_t seed) noexcept;
            virtual ~ParkMiller() noexcept;

            virtual uint32_t next32() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParkMiller);
            int32_t state;
        };
    }

}

#endif

