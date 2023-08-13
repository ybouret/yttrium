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
#if defined(Y_GNU) || defined(Y_MICROSOFT) || defined(Y_INTEL)
        return __Zero__;
#endif
    }


    void Sign:: Change(SignType &s) noexcept
    {
        s = Opposite(s);
    }

    void Sign:: MakeAbs(SignType &s) noexcept
    {
        switch(s)
        {
            case Negative: s = Positive; break;
            case __Zero__:
            case Positive:
                break;
        }
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
#if defined(Y_GNU) || defined(Y_MICROSOFT) || defined(Y_INTEL)
        return ZZ_Signs;
#endif
    }

    SignType Sign::Product(const SignType lhs, const SignType rhs) noexcept
    {
        switch(lhs)
        {
            case __Zero__: break;

            case Negative:
                switch(rhs)
                {
                    case __Zero__: break;
                    case Negative: return Positive;
                    case Positive: return Negative;
                }
                break;

            case Positive:
                switch(rhs)
                {
                    case __Zero__: break;
                    case Negative: return Negative;
                    case Positive: return Positive;
                }
                break;
        }
        return __Zero__;
    }

    bool Sign:: ZeroIsFoundIn(const PairOfSigns pair) noexcept
    {
        switch(pair)
        {
            case ZN_Signs:
            case ZZ_Signs:
            case ZP_Signs:
            case NZ_Signs:
            case PZ_Signs:
                return true;

            case NN_Signs:
            case PP_Signs:
            case NP_Signs:
            case PN_Signs:
                break;
        }
        return false;
    }

    bool Sign:: ZeroIsMissing(const PairOfSigns pair) noexcept
    {
        switch(pair)
        {
            case ZN_Signs:
            case ZZ_Signs:
            case ZP_Signs:
            case NZ_Signs:
            case PZ_Signs:
                return false;

            case NN_Signs:
            case PP_Signs:
            case NP_Signs:
            case PN_Signs:
                break;
        }
        return true;
    }
}
