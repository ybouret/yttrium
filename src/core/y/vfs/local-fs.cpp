
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

    bool LocalFS::tryRemoveFile(const String &path)
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
#include <sys/stat.h>

namespace Yttrium
{
    namespace
    {
        class LocalScanner : public VFS::Scanner
        {
        public:
            explicit LocalScanner(const VFS &fsys, const String &dirName) :
            VFS::Scanner(fsys,dirName), handle(Setup(dirName))
            {
            }

            virtual ~LocalScanner() noexcept
            {
                Y_GIANT_LOCK();
                assert(0 != handle);
                closedir(handle);
                Coerce(handle) = 0;
            }

            DIR * const handle;

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
                const dirent * const dp = readdir(handle);
                if (!dp) return 0;
                const String path = dir + dp->d_name;
                return new VFS::Entry(vfs,path);
            }
        };
    }

    VFS::EntryType LocalFS:: findEntryType(const String &path, bool &link) const
    {
        Y_GIANT_LOCK();
        assert(false==link);

        struct stat buf;
        mode_t &    m = buf.st_mode;

        Y_STATIC_ZVAR(buf);
        if(0!=lstat(path.c_str(),&buf))
        {
            return IsUnk;
        }

        if( S_ISLNK(m) )
        {
            //std::cerr << "->link" << std::endl;
            link = true;
            Y_STATIC_ZVAR(buf);
            if(0!=stat(path.c_str(),&buf))
            {
                return IsUnk;
            }
        }

        if( S_ISREG(m) )
        {
            return IsReg;
        }

        if( S_ISDIR(m) )
        {
            return IsDir;
        }

        return IsUnk;
    }

}

#endif


#if defined(Y_WIN)
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    class LocalScanner : public VFS::Scanner
    {
    public:
        explicit LocalScanner(const VFS &fsys, const String &dirName) :
        VFS::Scanner(fsys,dirName),
        fData(),
        hFind(INVALID_HANDLE_VALUE),
        ready(TRUE)
        {
            Y_GIANT_LOCK();
			const String wDirName = LocalFS::MakeWin32Path(dirName);
            memset(&fData, 0, sizeof(fData));
            hFind = ::FindFirstFile(wDirName(), &fData);
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
                AutoPtr<VFS::Entry> ep = new VFS::Entry(vfs, fData.cFileName);
                ready = ::FindNextFile(hFind, &fData);
                return ep.yield();
            }
            else
                return 0;
        }
    };

    VFS::EntryType LocalFS:: findEntryType(const String &path, bool &link) const
    {
		Y_GIANT_LOCK();
		link = false;
		const DWORD res = ::GetFileAttributes(path.c_str());
		if (INVALID_FILE_ATTRIBUTES == res)
		{
			return IsUnk;
		}
		//std::cerr << "res=" << res << std::endl;
		
		if (res & FILE_ATTRIBUTE_DIRECTORY)
			return IsDir;

		if (res & FILE_ATTRIBUTE_NORMAL)
			return IsReg;

        return IsUnk;
    }

}
#endif

namespace Yttrium
{


    VFS::Scanner *LocalFS:: openDirectory(const String &dirName)
    {
        return new LocalScanner(*this, dirName);
    }

}

#if defined(Y_BSD)
#include <sys/types.h>
#endif

namespace Yttrium
{

    void      LocalFS:: makeDirectory(const String &dirName, const bool mayExist)
    {
        Y_GIANT_LOCK();

#if defined(Y_BSD)
        if( mkdir( dirName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0)
        {
            const int err = errno;
            switch(err)
            {
                case EEXIST:
                    if(mayExist) return; // OK
                default:
                    break;
            }
            throw Libc::Exception(err, "mkdir('%s')", dirName.c_str() );
        }
        return;
#endif

#if defined(Y_WIN)
        if( ! ::CreateDirectory( dirName.c_str(), 0 ) )
        {
            const DWORD err = ::GetLastError();
            switch(err)
            {
                case ERROR_ALREADY_EXISTS:
                    if(mayExist) return; // OK
                default:
                    break;
            }
            throw Win32::Exception(err,"CreateDirectory('%s')", dirName.c_str() );
        }
        return;
#endif

        throw Specific::Exception("makeDirectory", "Not Implemented");
    }

    bool LocalFS:: tryEraseEmpty(const String &dirName)
    {
        Y_GIANT_LOCK();

#if defined(Y_BSD)
        if( rmdir( dirName.c_str() ) < 0 ) return false;
        return true;
#endif

#if defined(Y_WIN)
        if( ! ::RemoveDirectory( dirName.c_str()) ) return false;
        return true;
#endif

        throw Specific::Exception("tryEraseEmpty", "Not Implemented");

    }

}
