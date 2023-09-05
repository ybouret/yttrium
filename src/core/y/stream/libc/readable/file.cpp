#include "y/stream/libc/readable/file.hpp"
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

        ReadableFile:: ~ReadableFile() noexcept
        {
        }

        static inline void *openStdIn()
        {
            Y_GIANT_LOCK();
            FILE *fp = stdin;
            if(!fp) throw Libc::Exception(EIO,"closed stdin!");
            return fp;
        }

        ReadableFile:: ReadableFile(const StdIn_ &) :
        IsStdIn(),
        File(openStdIn(),false)
        {
        }

        static inline FILE * openCFILE(const char *fileName)
        {
            assert(0!=fileName);
            assert(0!=strcmp(fileName,Y_STDIN));

            Y_GIANT_LOCK();
            FILE *fp = fopen(fileName,"rb");
            if(!fp) throw Libc::Exception(errno,"fopen(%s)",fileName);
            return fp;

        }

        ReadableFile:: ReadableFile(const char *fileName) :
        IsStdIn(fileName),
        File( flag ? openStdIn() : openCFILE(fileName), !flag )
        {
        }

        ReadableFile:: ReadableFile(const String &fileName) :
        IsStdIn(fileName()),
        File( flag ? openStdIn() : openCFILE(fileName()), !flag )
        {
        }


    }

}

