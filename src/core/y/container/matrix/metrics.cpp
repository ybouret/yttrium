#include "y/container/matrix/metrics.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    Y_SHALLOW_IMPL(TransposeOf);

    const char * const MatrixMetrics:: CallSign = "Matrix";

    MatrixMetrics:: MatrixMetrics(const size_t nr, const size_t nc) :
    rows(nr),
    cols(nc),
    items(rows*cols)
    {
        if( ((rows>0) && (cols<=0) ) || ( (rows<=0) && (cols>0) ) )
        {
            throw Specific::Exception(CallSign,"invalid rows=%lu and cols=%lu", (unsigned long)rows, (unsigned long)cols );
        }
    }

    MatrixMetrics:: ~MatrixMetrics() noexcept
    {
        Coerce(rows) = 0;
        Coerce(cols) = 0;
    }

    MatrixMetrics:: MatrixMetrics(const MatrixMetrics &other) noexcept :
    rows(other.rows),
    cols(other.cols),
    items(other.items)
    {
    }

    MatrixMetrics:: MatrixMetrics(const MatrixMetrics &other, const TransposeOf_ &) noexcept :
    rows(other.cols),
    cols(other.rows),
    items(other.items)
    {

    }

    bool MatrixMetrics:: isSquare() const noexcept
    {
        return cols == rows;
    }

    bool MatrixMetrics:: isEmpty() const noexcept
    {
        return rows<=0;
    }

    bool MatrixMetrics:: isValid() const noexcept
    {
        return rows>0;
    }

    bool MatrixMetrics :: hasSameMetricsThan(const MatrixMetrics &other) const noexcept
    {
        return (rows==other.rows) && (cols==other.cols);
    }

    bool MatrixMetrics :: hasSameMetricsThanTransposeOf(const MatrixMetrics &other) const noexcept
    {
        return (rows==other.cols) && (cols==other.rows);
    }
    

}
