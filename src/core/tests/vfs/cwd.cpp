#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"



#include "y/stream/proc/input.hpp"
#include "y/stream/libc/output.hpp"



using namespace Yttrium;

namespace
{
    static inline void ls()
    {
        OutputFile fp(StdErr);
        std::cerr << "<listing>" << std::endl;
#if defined(Y_BSD)
        ProcInput::SendTo(fp, "ls");
#endif

#if defined(Y_WIN)
        ProcInput::SendTo(fp, "dir");
#endif

        std::cerr << "<listing/>" << std::endl;

    }
}

Y_UTEST(vfs_cwd)
{
    VFS  & vfs = LocalFS::Instance();
    String cwd = vfs.getCWD();
    std::cerr << "cwd=" << cwd << std::endl;
    ls();

    const char tmp[] = "temporary";
    vfs.makeDirectory(tmp, true);
    vfs.setCWD(tmp);
    ls();

}
Y_UDONE()

