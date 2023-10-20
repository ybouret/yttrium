
#include "y/vfs/vfs.hpp"
#include "y/text/ops.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(vfs_dname)
{
    for(int i=1;i<argc;++i)
    {
        const char * const path = argv[i];
        const size_t       size  = StringLength(path);
        const String       dpath(path,size);
        const String       dname = VFS::MakeDirName(path,size);
        std::cerr << "'" << dpath << "' => '" << dname << "'" << std::endl;

    }
}
Y_UDONE()

