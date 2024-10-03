#include "y/vfs/local/fs.hpp"
#include "y/utest/run.hpp"



#include "y/stream/proc/input.hpp"
#include "y/stream/libc/output.hpp"


using namespace Yttrium;
 
Y_UTEST(vfs_cwd)
{
    VFS  & fs = LocalFS::Instance();
    String cwd = fs.getCWD();
    std::cerr << "cwd=" << cwd << std::endl;
    {
        OutputFile fp(StdErr);
#if defined(Y_BSD)
        ProcInput::SendTo(fp, "ls");
#endif

#if defined(Y_WIN)
		ProcInput::SendTo(fp, "dir");
#endif
        
    }
}
Y_UDONE()

