#include "y/stream/io/variable-info.hpp"
#include "y/text/c-format.hpp"
#include <cstring>

namespace Yttrium
{
    namespace IO
    {

        void _VariableInfo:: clear() noexcept
        {
            memset(info,0,size);
        }

        _VariableInfo:: ~_VariableInfo() noexcept
        {
        }

        _VariableInfo:: _VariableInfo(char * const _info,
                                      const size_t _size) noexcept :
        info(_info),
        size(_size)
        {
            assert(0!=info);
            assert(size>0);
            clear();
        }

       void _VariableInfo:: format(const char * const fmt, void * const ptr) noexcept
        {
            C_Format::Buffer(info,size,fmt,ptr);
        }

        const char * _VariableInfo:: operator()(const char * const fmt,...) noexcept
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
            return info;
        }

        void _VariableInfo:: assign(const _VariableInfo &other) noexcept
        {
            if(other.size<=size)
            {
                memmove(info,other.info,other.size);
                memset(info+other.size,0,size-other.size);
            }
            else
            {
                // other.size>size
                memmove(info,other.info,size-1);
                info[size-1] = 0;
            }
        }



    }
}
