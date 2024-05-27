#include "y/apk/component.hpp"

namespace Yttrium
{
    namespace APK
    {

        template <typename T> static inline
        SignType CompareAssembly( const Assembly<T> &lhs, const Assembly<T> &rhs ) noexcept
        {
            switch( Sign::Of(lhs.count,rhs.count) )
            {
                case Negative: return Negative;
                case Positive: return Positive;
                case __Zero__: break;
            }
            size_t    n = lhs.count;
            const T * L = lhs.entry + n;
            const T * R = rhs.entry + n;
            while(n-- > 0)
            {
                switch( Sign::Of(*(--L), *(--R)) )
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__: break;
                }
            }
            return __Zero__;
        }



        SignType Component:: Compare(Component &lhs, Component &rhs) noexcept
        {
            switch( TuneUp(lhs,rhs) )
            {
                case AsBytes: return CompareAssembly(lhs.bytes, rhs.bytes);
                case AsNum16: return CompareAssembly(lhs.num16, rhs.num16);
                case AsNum32: return CompareAssembly(lhs.num32, rhs.num32);
                case AsNum64: return CompareAssembly(lhs.num64, rhs.num64);
            }
        }

    }

}


