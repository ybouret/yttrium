

//! \file

#ifndef Y_MKL_LAK_LU_Included
#define Y_MKL_LAK_LU_Included 1

#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class LU
        {
        public:
            explicit LU() noexcept;
            virtual ~LU() noexcept;

            bool build(Matrix<T> &a);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LU);
            class Code;
            Code *code;
        };

    }

}

#endif
