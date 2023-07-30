#include "y/container/matrix/row.hpp"

namespace Yttrium
{

    namespace Core
    {
        const char * const MatrixRow:: CallSign = "MatrixRow";
        
        MatrixRow:: ~MatrixRow() noexcept
        {
        }

        MatrixRow:: MatrixRow() noexcept {}

        MatrixRow::Info::Info(void *rp,const size_t nc) noexcept :
        addr(rp),
        cols(nc)
        {
            assert(cols>0);
        }

        MatrixRow::Info:: ~Info() noexcept
        {
        }

        void MatrixRow::Info:: move(const size_t blockSize) noexcept
        {
            assert(0!=addr);
            assert(blockSize>0);
            addr = static_cast<char *>(addr)+(cols*blockSize);
        }
    }

}
