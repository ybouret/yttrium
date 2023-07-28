#include "y/io/libc/file.hpp"
#include <cstdio>

namespace Yttrium
{
    Y_SHALLOW_IMPL(StdIn);
    Y_SHALLOW_IMPL(StdErr);
    Y_SHALLOW_IMPL(StdOut);

    namespace Libc
    {
        File:: File(void *user, const bool flag) noexcept :
        handle(user),
        mustClose(flag)
        {

        }

        File:: ~File() noexcept
        {
            assert(0!=handle);
            if(mustClose)
            {
                (void)fclose( static_cast<FILE*>(handle) );
                handle = 0;
            }
        }
    }

}
