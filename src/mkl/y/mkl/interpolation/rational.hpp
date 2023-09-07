
//! \file

#ifndef Y_MKL_Interpolation_Rational_Included
#define Y_MKL_Interpolation_Rational_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class RationalInterpolation
        {
        public:

            explicit RationalInterpolation() noexcept;
            explicit RationalInterpolation(const size_t);
            virtual ~RationalInterpolation() noexcept;

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
