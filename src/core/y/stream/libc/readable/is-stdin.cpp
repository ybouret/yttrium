
#include "y/stream/libc/readable/is-stdin.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Libc
    {
        IsStdIn:: ~IsStdIn() noexcept
        {

        }

        IsStdIn:: IsStdIn() noexcept : flag(true) {}



        IsStdIn:: IsStdIn(const char *fileName) :
        flag(false)
        {
            if( 0 == fileName) throw Specific::Exception("Libc::InputFile","NULL file name");
            if( 0 == strcmp(Y_STDIN,fileName) )
            {
                Coerce(flag) = true;
            }
        }
    }

}

