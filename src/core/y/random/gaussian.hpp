
// \file

#ifndef Y_Random_Gaussian_Included
#define Y_Random_Gaussian_Included 1

#include "y/random/bits.hpp"
#include "y/mkl/numeric.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Random
    {
        template <typename T>
        class Gaussian : public SharedBits
        {
        public:
            inline explicit Gaussian(const SharedBits &sharedBits) noexcept :
            SharedBits(sharedBits),
            indx(0),
            g0(0),
            g1(0)
            {
                BoxMuller();
            }

            inline T operator()(void) noexcept
            {
                switch(indx)
                {
                    case 0: indx=1; return g0;
                    default:
                        break;
                }
                indx = 0;
                const T g = g1;
                BoxMuller();
                return g;
            }

            inline virtual ~Gaussian() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gaussian);
            unsigned indx;
            T        g0,g1;

            inline void BoxMuller() noexcept
            {
                Random::Bits &ran = **this;
                T r = ran.to<T>();
                while(r <= MKL::Numeric<T>::EPSILON ) r = ran.to<T>();
                const T theta = MKL::Numeric<T>::PI * ran.symm<T>();
                const T ampli = std::sqrt(-Twice( std::log(r) ));
                g0 = ampli * std::sin(theta);
                g1 = ampli * std::cos(theta);
            }


        };
    }

}

#endif

