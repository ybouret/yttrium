#include "y/string/env.hpp"
#include "y/lockable.hpp"
#include "y/text/ops.hpp"
#include "y/system/exception.hpp"

#if defined(Y_BSD)
#include <cstdlib>
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
