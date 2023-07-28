
#include "y/io/libc/input.hpp"
#include "y/system/exception.hpp"

#include <cstdio>
#include <cerrno>

namespace Yttrium
{
    namespace Core { template <typename> class String; }

    namespace Libc
    {

        InputFile:: ~InputFile() noexcept
        {
        }

        static inline void *OpenStdIn()
        {
            FILE *fp = stdin;
            if(!fp) throw Libc::Exception(EIO,"closed stdin!");
            return fp;
        }

        InputFile:: InputFile(const StdIn_ &) :
        InputStream(),
        File(OpenStdIn(),false)
        {

        }

    }

}

