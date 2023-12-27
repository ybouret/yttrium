//! \file

#ifndef Y_MKL_Filter_ReSaGo_Included
#define Y_MKL_Filter_ReSaGo_Included 1


#include "y/mkl/api.hpp"
#include "y/container/matrix.hpp"
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //! Regular Savitzky-Golay Filters
        struct ReSaGo
        {
            static const size_t MaxKeptDegree = 2;

            class Filter : public Object
            {
            public:
                explicit Filter(const int64_t n, const int64_t j, const size_t degree);
                virtual ~Filter() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };

            class Factory
            {
            public:
                explicit Factory();
                virtual ~Factory() noexcept;

                const Matrix<apq> & operator()(const uint32_t nLeft,
                                               const uint32_t nRight,
                                               const uint32_t degree);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Factory);
                class Code;
                Code *code;
            };
        };
    }

}

#endif
