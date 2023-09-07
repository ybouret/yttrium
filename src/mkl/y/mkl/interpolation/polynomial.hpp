//! \file

#ifndef Y_MKL_Interpolation_Polynomial_Included
#define Y_MKL_Interpolation_Polynomial_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! Polynomial Interpolation
        //
        //
        //______________________________________________________________________
        template <typename T>
        class PolynomialInterpolation
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit PolynomialInterpolation() noexcept;    //!< setup empty
            explicit PolynomialInterpolation(const size_t); //!< setup with capacity
            virtual ~PolynomialInterpolation() noexcept;    //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! perform interpolation at x from [xa,ya]
            /**
             \param x value where to inter/extrapolator
             \param xa abscissae
             \param ya ordinates
             \param dy error estimate
             */
            T operator()(const T            x,
                         const Readable<T> &xa,
                         const Readable<T> &ya,
                         T                 &dy);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PolynomialInterpolation);
            class Code;
            Code *code;
        };

    }
}


#endif
