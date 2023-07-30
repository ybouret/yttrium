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

        MatrixRow::Info::Info(void *rp,const size_t nc) noexcept :
        addr(rp),
        cols(nc)
        {
            assert(cols>0);
        }

        MatrixRow::Info:: ~Info() noexcept
        {
        }
        
    }

}
