#include "y/string/env.hpp"
#include "y/lockable.hpp"
#include "y/text/ops.hpp"
#include "y/system/exception.hpp"

#if defined(Y_BSD)
#include <cstdlib>
#endif

#if defined(Y_WIN)
#include <windows.h>
#endif

namespace Yttrium
{
    bool Environment:: Get(String &value, const String &name)
    {
        Y_GIANT_LOCK();
        value.free();

#if defined(Y_BSD)
        const char *s = getenv( name.c_str() );
        if(!s) return false;
        value = s;
        return true;
#endif

#if defined(Y_WIN)
        LPCH es = ::GetEnvironmentStrings();
        if (!es) throw Win32::Exception(::GetLastError(), "GetEnvironmentStrings()");

        try
        {
            const char* curr = es;


        CYCLE:
            const char* next = curr;
            while (0 != *(++next))
                ;

            const size_t size = (next - curr);


            //std::cerr << "size=" << size << std::endl;
            if ( size <= 1 && 0 == *next)
            {
                // std::cerr << "done" << std::endl;
                return false;
            }
			const String content(curr, size);
			std::cerr << "content=" << content << std::endl;
            const char * const sep = strchr(curr, '=');
            if (0==sep) throw Win32::Exception(ERROR_NO_DATA, "while parsing environment strings");
            const String uuid(curr, sep - curr);
            if (uuid == name)
            {
				const char * const ini = sep + 1;
                const String data(ini, next - ini);
                value = data;
                return true;
            }
            
           //std::cerr << "|_uuid='" << uuid << "'" << std::endl;
            //std::cerr << "|_data='" << data << "'" << std::endl;

            //const String content(curr, size);
            //std::cerr << "|_[" << content << "]" << std::endl;
            curr = ++next;
            goto CYCLE;
        }
        catch (...)
        {
            ::FreeEnvironmentStrings(es);
            throw;
        }

        ::FreeEnvironmentStrings(es);
        return false;
#endif


    }

    bool Environment:: Get(String &value, const char * const name)
    {
        const String _(name);
        return Get(value,_);
    }

    bool Environment:: Flag(const String &name)
    {

        String value;
        if(!Get(value,name)) return false;

        TextOps::ToLower(&value[1], value.size());

        // true ?
        {
            static const char * const flag[] =
            {
                "1", "true", "on"
            };

            for(unsigned i=0;i<sizeof(flag)/sizeof(flag[0]);++i)
            {
                if(flag[i]==value) return true;
            }
        }

        // false ?
        {
            static const char * const flag[] =
            {
                "0", "false", "off"
            };

            for(unsigned i=0;i<sizeof(flag)/sizeof(flag[0]);++i)
            {
                if(flag[i]==value) return false;
            }
        }

        throw Specific::Exception("Environment::Flag","invalid value = '%s", value.c_str());

    }


    bool Environment::Flag(const char *const name)
    {
        const String _(name); return Flag(_);
    }

}
