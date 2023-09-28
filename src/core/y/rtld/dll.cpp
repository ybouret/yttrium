#include "y/rtld/dll.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/type/nullify.hpp"

#if defined(Y_BSD)
#include <dlfcn.h>
#define Y_DLL_HANDLE void *
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define Y_DLL_HANDLE HMODULE
#endif

namespace Yttrium
{

    const char * const DLL:: CallSign = "DLL";

    class DLL:: Code : public Object, public Counted
    {
    public:
        explicit Code(const char *soname) :
        Object(),
        Counted(),
        handle(Open(soname))
        {
        }

        virtual ~Code() noexcept
        {
            assert(0!=handle);
            assert(0==quantity());
            Y_GIANT_LOCK();
            
#if defined(Y_BSD)
            (void) dlclose(handle);
#endif

#if defined(Y_WIN)
            (void) ::FreeLibrary(handle);
#endif
            Coerce(handle) = 0;
        }

        Y_DLL_HANDLE const handle;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);

        static inline Y_DLL_HANDLE Open(const char *soname)
        {
            Y_GIANT_LOCK();
            if(!soname) throw Specific::Exception(DLL::CallSign,"Open(NULL)");

#if defined(Y_BSD)
            Y_DLL_HANDLE dll = dlopen(soname, RTLD_GLOBAL | RTLD_NOW);
            if(!dll)
            {
                throw Specific::Exception(DLL::CallSign, "%s", dlerror() );
            }
            return dll;
#endif

#if defined(Y_WIN)
            Y_DLL_HANDLE dll = ::LoadLibrary(soname);
            if(!dll)
            {
                throw Win32::Exception(::GetLastError(),"LoadLibrary('%s')", soname);
            }
            return dll;
#endif

        }
    };

}

#include "y/string.hpp"

namespace Yttrium
{
    DLL:: ~DLL() noexcept
    {
        assert(0!=code);
        if( code->liberate() )
            Nullify(code);

    }

    DLL:: DLL(const char *soname) :
    code( new Code(soname) )
    {
        code->withhold();
    }

    DLL:: DLL(const String &soname) :
    code( new Code(soname.c_str()) )
    { code->withhold(); }

    DLL:: DLL(const DLL &dll) noexcept : code( dll.code )
    { assert(0!=code); code->withhold(); }


    void * DLL:: symbol(const char *name) noexcept
    {
        assert(0!=name);
        assert(0!=code);
#if defined(Y_BSD)
        return dlsym(code->handle,name);
#endif

#if defined(Y_WIN)
        return (void*) ::GetProcAddress(code->handle,name);
#endif

    }

}
