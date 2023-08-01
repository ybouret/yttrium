
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
        //______________________________________________________________________
        //
        //
        //
        //! Guassian Random values
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Gaussian : public SharedBits
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            inline explicit Gaussian(const SharedBits &sharedBits) noexcept :
            SharedBits(sharedBits), i0(true), g0(0), g1(0)
            { BoxMuller(); }

            //! cleanup
            inline virtual ~Gaussian() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! return next Gaussian value
            inline T operator()(void) noexcept
            {
                if(i0)
                {
                    i0 = false;
                    return g0;
                }
                else
                {
                    i0 = true;
                    const T g = g1;
                    BoxMuller();
                    return g;
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gaussian);
            bool i0;
            T    g0,g1;

            //! compute two succesive values
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

