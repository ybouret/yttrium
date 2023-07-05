#include "y/random/bits.hpp"
#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace Random
    {

        template <typename T>
        class Metrics32
        {
        public:
            inline Metrics32() noexcept :
            shift(0),
            max32(0),
            denom(0)
            {
                static const uint64_t one64 = 1;
                //static const T        one   = 1;
                unsigned              msb = 32;
                uint64_t              top = one64 << msb;

                while(true)
                {
                    const T maxDen(top);
                    const T maxNum = T(top-1)+T(0.5);
                    if(maxNum/maxDen<T(1))
                    {
                        break;
                    }
                    --msb;
                    top >>= 1;
                }

                while(true)
                {
                    const T maxDen(top);
                    const T minNum = T(0.5);
                    if(minNum/maxDen>0)
                    {
                        break;
                    }
                    --msb;
                    top >>= 1;
                }

                Coerce(shift) = msb;
                Coerce(max32) = uint32_t(top-1);
                Coerce(denom) = T(top);

                std::cerr << "float" << std::setw(3) << sizeof(T)*8 << ": denom=2^" << shift << " = " << denom << " => max32=" << max32 << std::endl;

            }

            inline ~Metrics32() noexcept
            {
            }

            const unsigned shift;
            const uint32_t max32;
            const T        denom;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics32);
        };

        Bits:: ~Bits() noexcept
        {
        }


        static const Metrics32<float>       metricsF__;
        static const Metrics32<double>      metricsD__;
        static const Metrics32<long double> metricsL__;


        Bits:: Bits(const uint32_t maxU32) noexcept :
        umax(maxU32),
        metricsF( &metricsF__),
        metricsD( &metricsD__),
        metricsL( &metricsL__)
        {
            //std::cerr << metricsF->shift << std::endl;
        }


    }

}

