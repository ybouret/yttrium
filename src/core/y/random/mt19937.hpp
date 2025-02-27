
// \file

#ifndef Y_Random_mt19937_Included
#define Y_Random_mt19937_Included 1

#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {
        class MT19937 : public Random::Bits
        {
        public:
            static const size_t   N          = 624;
            static const size_t   M          = 397;
            static const size_t   MTI        = N+1;            //!< not initialized
            static const uint32_t MATRIX_A   = 0x9908b0dfUL;   //!< constant vector a
            static const uint32_t UPPER_MASK = 0x80000000UL;   //!< most significant w-r bits
            static const uint32_t LOWER_MASK = 0x7fffffffUL;   //!< least significant r bits

            static const uint32_t ReferenceKey[4];
            static const size_t   ReferenceLen = sizeof(ReferenceKey)/sizeof(ReferenceKey[0]);

            // C++
            explicit MT19937();
            explicit MT19937(const uint32_t s) noexcept;
            explicit MT19937(const uint32_t init_key[], const size_t key_length) noexcept;
            virtual ~MT19937() noexcept;


            //! generates a random number on [0,0xffffffff]-interval
            virtual uint32_t next32(void) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MT19937);
            size_t   mti;   //!< mti==N+1 means mt[N] is not initialized
            uint32_t mt[N]; //!< the array for the state vector

            void init_genrand(const uint32_t s) noexcept; //!< initializes mt[N] with a seed
            void init_by_array(const uint32_t init_key[], size_t key_length) noexcept; //!< initialize by an array with array-length
        };

    }

}

#endif


