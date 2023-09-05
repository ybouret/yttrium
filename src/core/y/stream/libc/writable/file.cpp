
#include "y/stream/libc/writable/file.hpp"
#include "y/string.hpp"

namespace Yttrium
{

    namespace Libc
    {

        WritableFile:: ~WritableFile() noexcept
        {
        }

        WritableFile:: WritableFile(const StdOut_ &_) :
        OutputGrasp(_),
        File( openOut(), false)
        {
        }

        WritableFile:: WritableFile(const StdErr_ &_) :
        OutputGrasp(_),
        File( openErr(), false)
        {
        }

        WritableFile:: WritableFile(const char *fileName, const bool append) :
        OutputGrasp(fileName),
        File( openFile(fileName,append), isReg )
        {
        }


        WritableFile:: WritableFile(const String &fileName, const bool append) :
        OutputGrasp(fileName()),
        File( openFile(fileName(),append), isReg )
        {
        }

        

    }
}
