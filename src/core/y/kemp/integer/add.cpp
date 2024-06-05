
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Integer Integer:: Add(const Integer &lhs, const Integer &rhs)
        {
            return Add(lhs.s,lhs.n,rhs.s,rhs.n);
        }

        Integer Integer:: Add(const Integer &lhs, const Natural &rhs)
        {
            return Add(lhs.s,lhs.n,rhs.sign(),rhs);
        }

        Integer Integer:: Add(const Natural &lhs, const Integer &rhs)
        {
            return Add(lhs.sign(),lhs,rhs.s,rhs.n);
        }

#if 0
        Integer Integer:: Add(const SignType ls, const Natural &ln, const SignType rs, const Natural &rn)
        {
            switch( Sign::MakePair(ls,rs) )
            {
                    // both zero => zero
                case ZZ_Signs: assert(ln<=0); assert(rn<=0);   break;

                    // rhs is zero => lhs
                case NZ_Signs:
                case PZ_Signs:
                    assert(ln>0);  assert(rn<=0);
                    return Integer(ls,ln);

                    // lhs is zero => rhs
                case ZN_Signs:
                case ZP_Signs:
                    assert(ln<=0); assert(rn>0);
                    return Integer(rs,rn);

                    // same signs => sign * (ln+rn)
                case PP_Signs:
                case NN_Signs:
                {
                    assert(ln>0); assert(rn>0); assert(ls==rs);
                    const Natural N = ln+rn;
                    return Integer(ls,N);
                }

                case PN_Signs: {
                    assert(ln>0); assert(rn>0);
                    assert(ls==Positive);
                    assert(rs==Negative);
                    switch( Natural::Compare(ln,rn) )
                    {
                        case Positive: { const Natural d = ln-rn; return Integer(Positive,d); }
                        case __Zero__: break;
                        case Negative: { const Natural d = rn-ln; return Integer(Negative,d); }
                    }
                } return Integer();

                case NP_Signs: {
                    assert(ln>0); assert(rn>0);
                    assert(ls==Negative);
                    assert(rs==Positive);
                    switch( Natural::Compare(ln,rn) )
                    {
                        case Positive: { const Natural d = ln-rn; return Integer(Negative,d); }
                        case __Zero__: break;
                        case Negative: { const Natural d = rn-ln; return Integer(Positive,d); }
                    }
                } return Integer();
            }

            assert(__Zero__==ls);
            assert(__Zero__==rs);
            return Integer();
        }
#endif
    }

}
