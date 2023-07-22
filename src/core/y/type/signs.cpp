#include "y/type/signs.hpp"

namespace Yttrium
{
    SignType Sign::Opposite(const SignType s) noexcept
    {
        switch(s)
        {
            case Negative: return Positive;
            case Positive: return Negative;
            case __Zero__: return __Zero__;
        }
        // never get here
#if defined(Y_GNU)
        return __Zero__;
#endif
    }


    void Sign:: Change(SignType &s) noexcept
    {
        s = Opposite(s);
    }


    PairOfSigns Sign:: MakePair(const SignType lhs, const SignType rhs) noexcept
    {
        switch(lhs)
        {
            case Negative:
                switch(rhs)
                {
                    case Negative: return NN_Signs;
                    case __Zero__: return NZ_Signs;
                    case Positive: return NP_Signs;
                }
                break;

            case __Zero__:
                switch(rhs)
                {
                    case Negative: return ZN_Signs;
                    case __Zero__: return ZZ_Signs;
                    case Positive: return ZP_Signs;
                }
                break;

            case Positive:
                switch(rhs)
                {
                    case Negative: return PN_Signs;
                    case __Zero__: return PZ_Signs;
                    case Positive: return PP_Signs;
                }
                break;
        }
        // never get here
#if defined(Y_GNU)
        return ZZ_Signs;
#endif
    }
}
