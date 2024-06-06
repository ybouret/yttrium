#ifndef Y_Kemp_Integer_Included
#error "not permitted"
#endif


//__________________________________________________________________
//
//! generic division
/**
 \param ls lhs sign
 \param ln rhs natural (Natural|uint64_t)
 \param rs rhs sign
 \param rn rhs natural (Natural|uint64_t)
 */
//__________________________________________________________________
template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
Integer Div(const SignType      ls,
            const LHS_UNSIGNED &ln,
            const SignType      rs,
            const RHS_UNSIGNED &rn)
{
    switch( Sign::MakePair(ls,rs) )
    {
            //------------------------------------------------------
            // zero denominator
            //------------------------------------------------------
        case ZZ_Signs:
        case NZ_Signs:
        case PZ_Signs:
            DivisionByZero();
            break;

            //------------------------------------------------------
            // zero result
            //------------------------------------------------------
        case ZN_Signs:
        case ZP_Signs:
            break;

            //------------------------------------------------------
            // Positive result
            //------------------------------------------------------
        case NN_Signs:
        case PP_Signs: {
            const Natural q = ln/rn;
            return Integer(Positive,q);
        }

            //------------------------------------------------------
            // Negative result
            //------------------------------------------------------
        case NP_Signs:
        case PN_Signs: {
            const Natural q = ln/rn;
            return Integer(Negative,q);
        }
    }
    return Integer();
    }
