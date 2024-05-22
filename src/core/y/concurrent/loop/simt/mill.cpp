
#include "y/concurrent/loop/simt/mill.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        SIMT_Mill:: ~SIMT_Mill() noexcept
        {
        }

        SIMT_Mill:: SIMT_Mill(const SharedLoop &sl) noexcept :
        mill(sl)
        {
        }

        
        Loop *       SIMT_Mill:: operator->()       noexcept { return & *mill; }
        const Loop * SIMT_Mill:: operator->() const noexcept { return & *mill; }


    }

}


