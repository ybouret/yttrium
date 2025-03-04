
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        template <typename T>
        static inline SignType CompareAssembly(const Assembly<T> &lhs,
                                               const Assembly<T> &rhs) noexcept
        {

            switch( Sign::Of(lhs.positive,rhs.positive) )
            {
                case Negative: return Negative;
                case Positive: return Positive;
                case __Zero__: break;
            }

            size_t   n = lhs.positive;
            const T *L = lhs.item + n;
            const T *R = rhs.item + n;

            while(n-- > 0 )
            {
                switch( Sign::Of( *(--L), *(--R) ) )
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__: continue;
                }
            }

            return __Zero__;
        }


        SignType Element:: Compare(Element &lhs, Element &rhs) noexcept
        {
            switch( TuneUp(lhs,rhs) )
            {
                case AsBytes: return CompareAssembly(lhs.bytes,rhs.bytes);
                case AsNum16: return CompareAssembly(lhs.num16,rhs.num16);
                case AsNum32: return CompareAssembly(lhs.num32,rhs.num32);
                case AsNum64: break;
            }
            return CompareAssembly(lhs.num64,rhs.num64);
        }

        SignType Element:: Compare(const Element &lhs, uint64_t qw) noexcept
        {
            switch(lhs.state)
            {
                case AsBytes: { const Bytes rhs(qw); return CompareAssembly(lhs.bytes,rhs); }
                case AsNum16: { const Num16 rhs(qw); return CompareAssembly(lhs.num16,rhs); }
                case AsNum32: { const Num32 rhs(qw); return CompareAssembly(lhs.num32,rhs); }
                case AsNum64: break;
            }
            { const Num64 rhs(qw); return CompareAssembly(lhs.num64,rhs); }

        }

        SignType Element:: Compare(uint64_t qw, const Element &rhs) noexcept
        {
            return Sign::Opposite( Compare(rhs,qw) );
        }
    }

}

