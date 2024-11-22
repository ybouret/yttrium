#include "y/stream/io/variable-info.hpp"
#include "y/text/c-format.hpp"
#include <cstring>

namespace Yttrium
{
    namespace IO
    {

        void VariableInfo_:: clear() noexcept
        {
            memset(info,0,size);
        }

        VariableInfo_:: ~VariableInfo_() noexcept
        {
        }

        VariableInfo_:: VariableInfo_(char * const _info,
                                      const size_t _size) noexcept :
        info(_info),
        size(_size)
        {
            assert(0!=info);
            assert(size>0);
            clear();
        }

       void VariableInfo_:: format(const char * const fmt, void * const ptr) noexcept
        {
            C_Format::Buffer(info,size,fmt,ptr);
        }

        const char * VariableInfo_:: operator()(const char * const fmt,...) noexcept
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            va_end(ap);
            return info;
        }

        void VariableInfo_:: assign(const VariableInfo_ &other) noexcept
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
