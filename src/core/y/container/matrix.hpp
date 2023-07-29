

//! \file

#ifndef Y_Matrix_Included
#define Y_Matrix_Included 1

#include "y/container/matrix/metrics.hpp"
#include "y/container/matrix/row.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/embedded.hpp"

namespace Yttrium
{

    template <class T, typename ALLOCATOR = Memory::Dyadic>
    class Matrix : public MatrixMetrics
    {
    public:

        inline explicit Matrix() noexcept : MatrixMetrics(0,0), code(0) {}

        inline explicit Matrix(const size_t nr, const size_t nc) :
        MatrixMetrics(nr,nc),
        code(0)
        {
            create();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Matrix);
        Memory::Embedded::Pointer code;

        inline void create()
        {
            if(items<=0) return;

        }
    };

}

#endif
