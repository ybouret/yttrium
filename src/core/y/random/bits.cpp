#include "y/random/bits.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/check/static.hpp"

#include <cmath>
#include <new>

namespace Yttrium
{
    namespace Random
    {


        template <typename T>
        class Metrics
        {
        public:
            inline Metrics() noexcept :
            loss(0),
            mask(0)
            {
                static const uint64_t one64 = 1;
                unsigned              msb   = 32;
                uint64_t              top   = one64 << msb;

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

                Coerce(loss)  = 32-msb;
                Coerce(mask)  = uint32_t(top-1);

            }

            inline ~Metrics() noexcept
            {
            }

            const unsigned loss;
            const uint32_t mask;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };

        template <typename T>
        class Bits:: Engine
        {
        protected:
            inline explicit Engine(const uint32_t _range,
                                   const uint32_t _umask,
                                   const T        _denom) noexcept :
            range(_range),
            umask(_umask),
            denom(_denom)
            {}

            static inline T Denom(uint64_t u) noexcept {
                return static_cast<T>(++u);
            }

        public:
            inline virtual ~Engine() noexcept {}

            virtual T compute(const uint32_t u) const noexcept = 0;

            const uint64_t range;
            const uint64_t umask;
            const T        denom;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };

        template <typename T>
        class FullEngine : public Bits::Engine<T>
        {
        public:
            using Bits::Engine<T>::Denom;
            using Bits::Engine<T>::range;
            using Bits::Engine<T>::denom;

            inline explicit FullEngine(const uint32_t _range) noexcept :
            Bits::Engine<T>(_range,0xffffffff,Denom(_range))
            {
            }

            inline virtual ~FullEngine() noexcept {}

            inline virtual T compute(const uint32_t u) const noexcept
            {
                static const T half(0.5);
                assert(u<=range);
                return (static_cast<T>(u)+half)/denom;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FullEngine);
        };

        template <typename T>
        class PackEngine : public Bits::Engine<T>
        {
        public:
            using Bits::Engine<T>::Denom;
            using Bits::Engine<T>::range;
            using Bits::Engine<T>::umask;
            using Bits::Engine<T>::denom;

            inline explicit PackEngine(const uint32_t _range,
                                       const uint32_t _umask) noexcept :
            Bits::Engine<T>(_range,_umask,Denom(_umask))
            {
            }

            inline virtual ~PackEngine() noexcept {}

            inline virtual T compute(const uint32_t u) const noexcept
            {
                static const T half(0.5);
                assert(u<=range);
                const uint32_t num = static_cast<uint32_t>( (uint64_t(u)*umask)/range );
                return (static_cast<T>(num)+half)/denom;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PackEngine);
        };










        template <typename T> static inline
        Bits::Engine<T> * BuildEngine(const uint32_t umax, void *wksp) noexcept
        {
            static const Metrics<T> metrics;
            if(umax>metrics.mask)
            {
                //std::cerr << "pack:float " << sizeof(T) << " : " << umax << " exceeds " << metrics.mask << std::endl;
                return new (wksp) PackEngine<T>(umax,metrics.mask);
            }
            else
            {
                //std::cerr << "full:float " << sizeof(T) << " : " << umax << " lower than " << metrics.mask << std::endl;
                return new (wksp) FullEngine<T>(umax);
            }
            return 0;
        }


        Bits:: ~Bits() noexcept
        {
        }







        Bits:: Bits(const uint32_t umax) noexcept :
        F(0),
        D(0),
        L(0),
        wkspF(),
        wkspD(),
        wkspL()
        {
            //std::cerr << "sizeof(FullEngine<float>)       = " << sizeof(FullEngine<float>) << std::endl;
            //std::cerr << "sizeof(FullEngine<double>)      = " << sizeof(FullEngine<double>) << std::endl;
            //std::cerr << "sizeof(FullEngine<long double>) = " << sizeof(FullEngine<long double>) << std::endl;
            //std::cerr << "sizeof(PackEngine<float>)       = " << sizeof(PackEngine<float>) << std::endl;
            //std::cerr << "sizeof(PackEngine<double>)      = " << sizeof(PackEngine<double>) << std::endl;
            //std::cerr << "sizeof(PackEngine<long double>) = " << sizeof(PackEngine<long double>) << std::endl;

            Coerce(F) = BuildEngine<float>(       umax ,Y_STATIC_ZARR(wkspF));
            Coerce(D) = BuildEngine<double>(      umax, Y_STATIC_ZARR(wkspD));
            Coerce(L) = BuildEngine<long double>( umax, Y_STATIC_ZARR(wkspL));
        }

        
        template <> float Bits:: to<float>() noexcept
        {
            return F->compute( next32() );
        }

        template <> double Bits:: to<double>() noexcept
        {
            return D->compute( next32() );
        }

        template <> long double Bits:: to<long double>() noexcept
        {
            return L->compute( next32() );
        }

        template <> uint8_t Bits:: to<uint8_t>() noexcept
        {
            return static_cast<uint8_t>( floor(255.0 * to<double>() + 0.5) );
        }

        template <> uint16_t Bits:: to<uint16_t>() noexcept
        {
            return static_cast<uint16_t>( floor(65535.0 * to<double>() + 0.5) );
        }

        template <> uint32_t Bits:: to<uint32_t>() noexcept
        {
            return static_cast<uint32_t>( floor(4294967295 * to<double>() + 0.5) );
        }

        template <> uint64_t Bits:: to<uint64_t>() noexcept
        {
            union {
                uint64_t qw;
                uint32_t dw[2];
            } alias =  { 0 };
            alias.dw[0] = to<uint32_t>();
            alias.dw[1] = to<uint32_t>();
            return alias.qw;
        }

    }

}

#include <cstdlib>

namespace Yttrium
{
    namespace Random
    {
        Rand:: Rand() noexcept : Bits(RAND_MAX)
        {
        }

        Rand:: ~Rand() noexcept
        {
        }

        uint32_t Rand:: next32() noexcept
        {
            return rand();
        }
    }

}

