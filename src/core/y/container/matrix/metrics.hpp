//! \file

#ifndef Y_Matrix_Metrics_Included
#define Y_Matrix_Metrics_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    class MatrixMetrics
    {
    public:
        static const char * const CallSign; //!< "Matrix"

        explicit MatrixMetrics(const size_t nr, const size_t nc);
        virtual ~MatrixMetrics() noexcept;
        explicit MatrixMetrics(const MatrixMetrics &) noexcept;

        bool isSquare() const noexcept;
        bool isValid()  const noexcept;
        bool isEmpty()  const noexcept;

        const size_t rows;
        const size_t cols;
        const size_t items;

    private:
        Y_DISABLE_ASSIGN(MatrixMetrics);
    };
}

#endif
