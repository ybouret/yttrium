#include "y/random/bits.hpp"
#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace Random
    {

        template <typename T>
        class Eval32
        {

        protected:
            inline explicit Eval32() noexcept {}

        public:
            inline virtual ~Eval32() noexcept {}

            virtual T conv(const uint32_t) const noexcept = 0;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Eval32);
        };

        template <typename T> class Pack32 : public Eval32<T>
        {
        public:
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pack32);
        };


        template <typename T> class Full32 : public Eval32<T>
        {
        public:
            static const T denom;

            inline explicit Full32() noexcept : Eval32<T>()
            {

            }
            inline virtual ~Full32() noexcept {}

            inline virtual T conv(const uint32_t u) const noexcept
            {
                static const T half(0.5);
                return ( T(u) + half ) / denom;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Full32);
        };

        template <> const float       Full32<float>::       denom = 4294967296.0f;
        template <> const double      Full32<double>::      denom = 4294967296.0;
        template <> const long double Full32<long double>:: denom = 4294967296.0L;




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

                Coerce(shift) = 32-msb;
                Coerce(max32) = uint32_t(top-1);
                Coerce(denom) = T(top);

                std::cerr << "float" << std::setw(3) << sizeof(T)*8 << ": denom=2^" << (32-shift) << " = " << denom << " => max32=" << max32 << std::endl;

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

        static const Full32<float>       full32F__;
        static const Full32<double>      full32D__;
        static const Full32<long double> full32L__;


        Bits:: Bits(const uint32_t maxU32) noexcept :
        umax(maxU32),
        metricsF( &metricsF__),
        metricsD( &metricsD__),
        metricsL( &metricsL__)
        {
        }


    }

}

