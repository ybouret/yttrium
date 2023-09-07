
//! \file

#ifndef Y_MKL_Interpolation_Rational_Included
#define Y_MKL_Interpolation_Rational_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Rational Interpolation
        //
        //
        //______________________________________________________________________
        template <typename T>
        class RationalInterpolation
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit RationalInterpolation() noexcept;     //!< setup empty
            explicit RationalInterpolation(const size_t);  //!< setup with capacity
            virtual ~RationalInterpolation() noexcept;     //!< cleanup

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
            Y_DISABLE_COPY_AND_ASSIGN(RationalInterpolation);
            class Code;
            Code *code;
        };

    }
}


#endif
