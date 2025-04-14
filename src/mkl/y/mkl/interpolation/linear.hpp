
//! \file

#ifndef Y_MKL_Interpolation_Linear_Included
#define Y_MKL_Interpolation_Linear_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    namespace MKL
    {

        template <typename T>
        class LinearInterpolation
        {
        public:

            explicit LinearInterpolation() noexcept;
            virtual ~LinearInterpolation() noexcept;

            T operator()(const T            x,
                         const Readable<T> &xa,
                         const Readable<T> &ya) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearInterpolation);
        };


    }

}

#endif
