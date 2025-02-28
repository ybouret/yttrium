
// \file

#ifndef Y_Random_mt19937_Included
#define Y_Random_mt19937_Included 1

#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Random
    {

        //______________________________________________________________________
        //
        //
        //
        //! Mersenne Twister Implemententaion
        //
        //
        //______________________________________________________________________
        class MT19937 : public Random::Bits
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t   N          = 624;            //!< internal space
            static const size_t   M          = 397;            //!< internal value
            static const size_t   MTI        = N+1;            //!< "not initialized" value
            static const uint32_t MATRIX_A   = 0x9908b0dfUL;   //!< constant vector a
            static const uint32_t UPPER_MASK = 0x80000000UL;   //!< most significant w-r bits
            static const uint32_t LOWER_MASK = 0x7fffffffUL;   //!< least significant r bits

            static const uint32_t ReferenceKey[4]; //!< reference key for testing
            static const size_t   ReferenceLen = sizeof(ReferenceKey)/sizeof(ReferenceKey[0]); //!< alias

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            explicit MT19937();                          //!< setup with system seed
            explicit MT19937(const uint32_t s) noexcept; //!< setup with seed
            virtual ~MT19937()                 noexcept; //!< cleanup

            //! setup with array of words
            explicit MT19937(const uint32_t init_key[], const size_t key_length) noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

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


