#include "y/random/park-miller.hpp"
#include "y/system/seed.hpp"

namespace Yttrium
{
    namespace Random
    {
//#define AM (1.0/IM)

        static const int32_t IA   = 16807;
        static const int32_t IM   = 2147483647;
        static const int32_t IQ   = 127773;
        static const int32_t IR   = 2836;
        static const int32_t MASK = 123459876;

        ParkMiller:: ParkMiller(const int32_t seed) noexcept :
        Bits(IM-1),
        state(seed)
        {

        }

        ParkMiller:: ParkMiller() noexcept :
        Bits(IM-1),
        state( SystemSeed::Get() )
        {

        }

        ParkMiller:: ~ParkMiller() noexcept
        {

        }

        uint32_t ParkMiller:: next32() noexcept
        {
            state ^= MASK;
            const int32_t k = state/IQ;
            state = IA*(state-k*IQ)-IR*k;
            if (state < 0) state += IM;
            const uint32_t res(state);
            state ^= MASK;
            return res;
        }



    }

}

