
#include "y/vfs/local-fs.hpp"
#include "y/system/exception.hpp"
#include <unistd.h>

namespace Yttrium
{
    const char * const LocalFS:: CallSign = "LocalFS";
    
    LocalFS:: ~LocalFS() noexcept
    {
    }

    LocalFS:: LocalFS() noexcept :
    Singleton<LocalFS>(),
    VFS()
    {
    }

    bool LocalFS:: TryRemove(const String &path)
    {
        Y_GIANT_LOCK();
#if defined(Y_BSD)
        if( 0!=unlink( path() ) ) return false;
        return true;
#endif

        throw Specific::Exception(CallSign,"TryRemove not implemented");
    }


    
}

#include "y/system/exception.hpp"

#if defined(Y_BSD)
#include <dirent.h>
#include <cerrno>

namespace Yttrium
{
    namespace
    {
        class LocalScanner : public VFS::Scanner
        {
        public:
            explicit LocalScanner(const VFS &fsys, const String &dirName) :
            VFS::Scanner(fsys), dir(Setup(dirName))
            {
            }

            virtual ~LocalScanner() noexcept
            {
                assert(0!=dir);
                closedir(dir);
                dir=0;
            }

            DIR *dir;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LocalScanner);

            static inline
            DIR *Setup(const String &dirName)
            {
                Y_GIANT_LOCK();
                DIR *d = opendir(dirName());
                if(!d)
                {
                    throw Libc::Exception(errno,"opendir(%s)", dirName());
                }
                return d;
            }

            virtual VFS::Entry *get()
            {
                Y_GIANT_LOCK();
                assert(0!=dir);
                const dirent *dp = readdir(dir);
                if(!dp) return 0;
                return new VFS::Entry(fs,dp->d_name);
            }
        };
    }
}

#endif


namespace Yttrium
{


    VFS::Scanner *LocalFS:: OpenDirectory(const String &dirName)
    {
        return new LocalScanner(*this,dirName);
    }

}
