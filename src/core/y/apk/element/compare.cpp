
#include "y/apk/element.hpp"
#include "y/system/error.hpp"
#include "y/text/hexadecimal.hpp"

namespace Yttrium
{
    namespace APK
    {



        template <typename T> static inline
        SignType CompareData(const void * const lhs,
                             const void * const rhs,
                             const size_t       n) noexcept
        {
            assert(0!=lhs);
            assert(0!=rhs);
            const T * L = static_cast<const T *>(lhs) + n;
            const T * R = static_cast<const T *>(rhs) + n;

#if 0
            if(n>1)
            {
                std::cerr << "Compare Data As<uint" << sizeof(T) * 8 << "_t>(" << n << ")" << std::endl;
                Core::Display(std::cerr << "lhs=", L, n, Hexadecimal::From<T>) << std::endl;
                Core::Display(std::cerr << "rhs=", R, n, Hexadecimal::From<T>) << std::endl;

            }
#endif

            for(size_t i=n;i>0;--i)
            {
                switch( Sign::Of( *(--L), *(--R) ) )
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__: break;
                }
            }

            return __Zero__;
        }


#define Y_RETURN_ELEMENT_COMPARE(LHS,RHS,TYPE) \
switch( Sign::Of(LHS,RHS) ) {\
case Negative: return Negative; \
case Positive: return Positive; \
case __Zero__: return CompareData<TYPE>(lhs.entry,rhs.entry,LHS);\
} break


        SignType Element:: Compare(Element &lhs, Element &rhs) noexcept
        {

            switch( TuneUp(lhs,rhs) )
            {
                case AsBytes: Y_RETURN_ELEMENT_COMPARE(lhs.bytes,rhs.bytes, uint8_t);
                case AsNum16: Y_RETURN_ELEMENT_COMPARE(lhs.num16,rhs.num16,uint16_t);
                case AsNum32: Y_RETURN_ELEMENT_COMPARE(lhs.num32,rhs.num32,uint32_t);
                case AsNum64: Y_RETURN_ELEMENT_COMPARE(lhs.num64,rhs.num64,uint64_t);
            }
            Libc::CriticalError(EINVAL, "%s::Compare(Element,Element)", CallSign);
            return __Zero__;
        }



        SignType Element:: Compare(Element &lhs, const uint64_t R) noexcept
        {

            if( lhs.set(AsNum64).num64 > 1 )
            {
                return Positive;
            }
            else
            {
                const uint64_t L = *static_cast<const uint64_t *>(lhs.entry);
                return Sign::Of(L,R);
            }

        }

        SignType Element:: Compare(uint64_t L, Element &rhs) noexcept
        {
            if( rhs.set(AsNum64).num64 > 1 )
            {
                return Negative;
            }
            else
            {
                const uint64_t R = *static_cast<const uint64_t *>(rhs.entry);
                return Sign::Of(L,R);
            }
        }



    }

}

