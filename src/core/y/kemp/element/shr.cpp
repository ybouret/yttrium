
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        template <typename T>
        static inline size_t SHR(Assembly<T> &arg) noexcept
        {
            static const unsigned lo2hi = sizeof(T)*8-1;
            T mask = 0x00;
            for(size_t i=arg.positive;i>0;)
            {
                T       &word = arg.item[--i];
                const T  next = (word&1);
                word >>= 1;
                word |=  mask;
                mask = next << lo2hi;
            }
            return arg.updateBits();
        }

        Element & Element:: shr() noexcept
        {
            switch(state)
            {
                case AsBytes: bits = SHR(bytes); break;
                case AsNum16: bits = SHR(num16); break;
                case AsNum32: bits = SHR(num32); break;
                case AsNum64: bits = SHR(num64); break;
            }
            
            return * revise();
        }

    }
}
