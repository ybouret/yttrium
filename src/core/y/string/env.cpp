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
		DWORD dw = ::GetEnvironmentVariable(name.c_str(), 0, 0);
		if (dw <= 0) return false;
        String res(dw-1, AsCapacity, true);
		const DWORD rd = ::GetEnvironmentVariable(name.c_str(), &res[1], dw);
        if (rd <= 0)
			throw Exception("unexpected vanished '%s'", name.c_str());
		value.swapWith(res);
		return true;
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

        throw Specific::Exception("Environment::Flag","invalid value = '%s'", value.c_str());

    }


    bool Environment::Flag(const char *const name)
    {
        const String _(name); return Flag(_);
    }

}
