#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"


using namespace Yttrium;

Y_UTEST(vfs_rm)
{
    const char * const fn = "sample.tmp";
    const String       fileName = fn;

    VFS &fs = LocalFS::Instance();
    {
        OutputFile::Overwrite(fn);
        fs.tryRemoveFile("sample.tmp");
    }
    {
        OutputFile::Overwrite(fileName);
        fs.tryRemoveFile(fileName);
    }

}
Y_UDONE()

