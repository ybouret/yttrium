
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



    int Formatted:: ComputeSize(const char *fmt, void *ptr) noexcept
    {
        assert(0!=fmt);
        assert(0!=ptr);
        va_list &ap = *(va_list *)ptr;
        return vsnprintf(NULL,0,fmt,ap);
    }

    int Formatted:: WriteString(String &ans, const char * fmt, void *ptr) noexcept
    {
        assert(0!=fmt);
        assert(0!=ptr);
        va_list &ap = *(va_list *)ptr;
        return vsnprintf( static_cast<char *>(ans.rw_addr()),ans.size()+1,fmt,ap);
    }

    String Formatted:: Get(const char *fmt,...)
    {
        assert(fmt!=NULL);
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = ComputeSize(fmt,&ap);
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
            const int chk = WriteString(ans,fmt,&ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            return ans;
        }
    }

    String * Formatted:: New(const char *fmt,...)
    {
        assert(fmt!=NULL);
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = ComputeSize(fmt,&ap);
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
            const int chk = WriteString(*ans,fmt,&ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            return ans.yield();
        }
    }


}
