#ifndef Y_Kemp_Integer_Included
#error "not permitted""
#endif

//__________________________________________________________________
//
//! generic multiplication
/**
 \param ls lhs sign
 \param ln rhs natural (Natural|uint64_t)
 \param rs rhs sign
 \param rn rhs natural (Natural|uint64_t)
 */
//__________________________________________________________________
template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
Integer Mul(const SignType      ls,
            const LHS_UNSIGNED &ln,
            const SignType      rs,
            const RHS_UNSIGNED &rn)
{
    switch( Sign::MakePair(ls,rs) )
    {
            //------------------------------------------------------
            // trivial __Zero__
            //------------------------------------------------------
        case ZZ_Signs:
        case ZN_Signs:
        case ZP_Signs:
        case NZ_Signs:
        case PZ_Signs:
            break;

            //------------------------------------------------------
            // Positive Result
            //------------------------------------------------------
        case PP_Signs:
        case NN_Signs: { const Natural p = ln * rn; return Integer(Positive,p); }

            //------------------------------------------------------
            // Negative Result
            //------------------------------------------------------
        case PN_Signs:
        case NP_Signs: { const Natural p = ln * rn; return Integer(Negative,p); }

    }
    return Integer();
}
