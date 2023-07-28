

#include "y/io/libc/output.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/string.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>

namespace Yttrium
{

    namespace Libc
    {
        OutputGrasp:: ~OutputGrasp() noexcept
        {

        }


        OutputGrasp:: OutputGrasp(const StdErr_ &) noexcept :
        isErr(true),
        isOut(false),
        isReg(false)
        {

        }

        OutputGrasp:: OutputGrasp(const StdOut_ &) noexcept :
        isErr(false),
        isOut(true),
        isReg(false)
        {

        }

        OutputGrasp:: OutputGrasp(const char *fileName):
        isErr(false),
        isOut(false),
        isReg(true)
        {
            if( 0 == fileName) throw Specific::Exception("Libc::OutputFile","NULL file name");

            if( 0 == strcmp(fileName,Y_STDERR) )
            {
                Coerce(isErr) = true;
                Coerce(isReg) = false;
                return;
            }

            if( 0 == strcmp(fileName,Y_STDOUT) )
            {
                Coerce(isOut) = true;
                Coerce(isReg) = false;
                return;
            }
            
        }


    }

    namespace Libc
    {

        OutputFile:: ~OutputFile() noexcept
        {
        }

    }

}


