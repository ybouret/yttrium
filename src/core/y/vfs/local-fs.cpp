
#include "y/vfs/local-fs.hpp"
#include "y/system/exception.hpp"

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(Y_BSD)
#include <unistd.h>
#endif

namespace Yttrium
{
    const char * const LocalFS::CallSign = "LocalFS";

    LocalFS:: ~LocalFS() noexcept
    {
    }

    LocalFS::LocalFS() noexcept :
    Singleton<LocalFS>(),
    VFS()
    {
    }

    bool LocalFS::TryRemove(const String &path)
    {
        Y_GIANT_LOCK();
#if defined(Y_BSD)
        if (0 != unlink(path())) return false;
        return true;
#endif

#if defined(Y_WIN)
        if (!::DeleteFile(path())) return false;
        return true;
#endif
        throw Specific::Exception(CallSign, "TryRemove not implemented");
    }



}

#include "y/container/algo/trim.hpp"

namespace Yttrium
{
    String LocalFS:: MakeWin32Path(const String &dirName)
    {
        static const char xtnd[] = { '\\', '*', 0, 0 };

        String res = dirName;
        Algo::Trim(res,IsSeparator);
        res.pushAtTail(xtnd,3);
        return res;
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
                Y_GIANT_LOCK();
                assert(0 != dir);
                closedir(dir);
                dir = 0;
            }

            DIR *dir;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LocalScanner);

            static inline DIR *Setup(const String &dirName)
            {
                Y_GIANT_LOCK();
                DIR *d = opendir(dirName());
                if (!d)
                {
                    throw Libc::Exception(errno, "opendir(%s)", dirName());
                }
                return d;
            }

            virtual VFS::Entry *get()
            {
                Y_GIANT_LOCK();
                assert(0 != dir);
                const dirent *dp = readdir(dir);
                if (!dp) return 0;
                return new VFS::Entry(fs, dp->d_name);
            }
        };
    }
}

#endif


#if defined(Y_WIN)
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    String MakeWin32Path(const String &dirName)
    {
        String res = dirName;

    }

    class LocalScanner : public VFS::Scanner
    {
    public:
        explicit LocalScanner(const VFS &fsys, const String &dirName) :
        VFS::Scanner(fsys),
        fData(),
        hFind(INVALID_HANDLE_VALUE),
        ready(TRUE)
        {
            Y_GIANT_LOCK();
            std::cerr << "opening " << dirName << std::endl;
            memset(&fData, 0, sizeof(fData));
            hFind = ::FindFirstFile(dirName(), &fData);
            if (INVALID_HANDLE_VALUE == hFind)
            {
                throw Win32::Exception(::GetLastError(), "FindFirstFile(%s)", dirName());
            }

        }

        virtual ~LocalScanner() noexcept
        {
            Y_GIANT_LOCK();
            ::FindClose(hFind);
            hFind = INVALID_HANDLE_VALUE;
            memset(&fData, 0, sizeof(fData));
        }

        WIN32_FIND_DATA fData;
        HANDLE          hFind;
        int             ready;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LocalScanner);

        virtual VFS::Entry *get()
        {
            Y_GIANT_LOCK();
            if (ready)
            {
                AutoPtr<VFS::Entry> ep = new VFS::Entry(fs, fData.cFileName);
                ready = ::FindNextFile(hFind, &fData);
                return ep.yield();
            }
            else
                return 0;
        }
    };

}
#endif

namespace Yttrium
{


    VFS::Scanner *LocalFS::OpenDirectory(const String &dirName)
    {
        return new LocalScanner(*this, dirName);
    }

}
