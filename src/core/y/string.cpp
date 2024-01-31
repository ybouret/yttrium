
#include "y/string.hpp"
#include "y/lockable.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace Yttrium
{
    static const char fn[] = "vsnprintf";

    String Formatted:: Get(const char *fmt,...)
    {

        assert(fmt!=NULL);
        Y_GIANT_LOCK();
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = vsnprintf(NULL,0,fmt,ap);
            va_end(ap);
            if(res<0) throw Libc::Exception(errno,"%s(...)",fn);
        }
        if(res<=0)
        {
            return String();
        }
        else
        {
            String  ans(res,AsCapacity,true); assert( ans.size() == size_t(res) );
            va_list ap;
            va_start(ap,fmt);
            const int chk = vsnprintf(&ans[1],ans.size()+1,fmt,ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            return ans;
        }
    }

    String * Formatted:: New(const char *fmt,...)
    {
        assert(fmt!=NULL);
        Y_GIANT_LOCK();
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = vsnprintf(NULL,0,fmt,ap);
            va_end(ap);
            if(res<0) throw Libc::Exception(errno,"%s(...)",fn);
        }
        if(res<=0)
        {
            return new String();
        }
        else
        {
            AutoPtr<String> ans = new String(res,AsCapacity,true); assert( ans->size() == size_t(res) );
            va_list ap;
            va_start(ap,fmt);
            const int chk = vsnprintf(& (*ans)[1],ans->size()+1,fmt,ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            return ans.yield();
        }
    }


}
