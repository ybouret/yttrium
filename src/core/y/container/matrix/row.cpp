#include "y/container/matrix/row.hpp"

namespace Yttrium
{

    namespace Core
    {
        const char * const MatrixRow:: CallSign = "MatrixRow";
        
        MatrixRow:: ~MatrixRow() noexcept
        {
            Coerce(cols) = 0;
        }

        MatrixRow:: MatrixRow(const size_t nc) noexcept :
        cols(nc)
        {
            assert(cols>0);
        }

    }

}
