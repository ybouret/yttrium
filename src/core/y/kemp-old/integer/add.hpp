#ifndef Y_Kemp_Integer_Included
#error "not permitted"
#endif


//__________________________________________________________________
//
//! generic addition
/**
 \param ls lhs sign
 \param ln rhs natural (Natural|uint64_t)
 \param rs rhs sign
 \param rn rhs natural (Natural|uint64_t)
 */
//__________________________________________________________________
template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
Integer Add(const SignType      ls,
            const LHS_UNSIGNED &ln,
            const SignType      rs,
            const RHS_UNSIGNED &rn)
{
    switch( Sign::MakePair(ls,rs) )
    {
            //------------------------------------------------------
            // special case
            //------------------------------------------------------
        case ZZ_Signs: assert(0==ln); assert(0==rn); break;

            //------------------------------------------------------
            // special case with one ZERO
            //------------------------------------------------------
        case NZ_Signs: return Integer(ls,ln);
        case PZ_Signs: return Integer(ls,ln);
        case ZN_Signs: return Integer(rs,rn);
        case ZP_Signs: return Integer(rs,rn);

            //------------------------------------------------------
            // all POSITIVE:
            //------------------------------------------------------
        case PP_Signs: { const Natural sum = ln+rn; return Integer(Positive,sum); }

            //------------------------------------------------------
            // all NEGATIVE:
            //------------------------------------------------------
        case NN_Signs: { const Natural sum = ln+rn; return Integer(Negative,sum); }

            //------------------------------------------------------
            // mixed case
            //------------------------------------------------------
        case PN_Signs:  {
            assert(ln>0); assert(rn>0);
            assert(ls==Positive);
            assert(rs==Negative);
            switch( Natural::Compare(ln,rn) )
            {
                case Positive: { const Natural dif = ln-rn; return Integer(Positive,dif); }
                case __Zero__: break;
                case Negative: { const Natural dif = rn-ln; return Integer(Negative,dif); }
            }
        } return Integer();

            //------------------------------------------------------
            // mixed case
            //------------------------------------------------------
        case NP_Signs: {
            assert(ln>0); assert(rn>0);
            assert(ls==Negative);
            assert(rs==Positive);
            switch( Natural::Compare(ln,rn) )
            {
                case Positive: { const Natural dif = ln-rn; return Integer(Negative,dif); }
                case __Zero__: break;
                case Negative: { const Natural dif = rn-ln; return Integer(Positive,dif); }
            }
        } return Integer();

    }
    return Integer();
}
