
#include "y/io/gzip/file.hpp"
#include "y/zlib/zlib.h"
#include "y/lockable.hpp"
#include <cerrno>
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace GZip
    {
        File:: ~File() noexcept
        {
            Y_GIANT_LOCK();
            assert(0!=handle);
            gzclose( static_cast<gzFile>(handle) );
            handle = 0;
        }

        File:: File(const char *fileName, const char *options) :
        handle(0)
        {
            assert(0!=fileName);
            assert(0!=options);
            Y_GIANT_LOCK();
            gzFile fp = gzopen(fileName,options);
            if(Z_NULL==fp)
            {
                throw Libc::Exception(errno,"gzopen(%s,%s)",fileName,options);
            }
            handle = (void*)fp;
        }

    }
}
