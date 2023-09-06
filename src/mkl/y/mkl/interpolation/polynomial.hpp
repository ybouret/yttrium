//! \file

#ifndef Y_MKL_Interpolation_Polynomial_Included
#define Y_MKL_Interpolation_Polynomial_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class PolynomialInterpolation
        {
        public:

            explicit PolynomialInterpolation() noexcept;
            explicit PolynomialInterpolation(const size_t);
            virtual ~PolynomialInterpolation() noexcept;

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
