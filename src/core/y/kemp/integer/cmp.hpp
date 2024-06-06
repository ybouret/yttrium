#ifndef Y_Kemp_Integer_Included
#error "not permitted"
#endif

//__________________________________________________________________
//
//! generic comparison with NO EXCEPTION
/**
 \param ls lhs sign
 \param ln rhs natural (Natural|uint64_t)
 \param rs rhs sign
 \param rn rhs natural (Natural|uint64_t)
 */
//__________________________________________________________________
template <typename LHS_UNSIGNED, typename RHS_UNSIGNED> static inline
SignType Cmp(const SignType      ls,
             const LHS_UNSIGNED &ln,
             const SignType      rs,
             const RHS_UNSIGNED &rn) noexcept
{
    switch( Sign::MakePair(ls,rs) )
    {
            //------------------------------------------------------
            // special case
            //------------------------------------------------------
        case ZZ_Signs: assert(0==ln); assert(0==rn); break;

            //------------------------------------------------------
            // trivial POSITIVE signs ls > rs
            //------------------------------------------------------
        case PZ_Signs: return Positive;
        case PN_Signs: return Positive;
        case ZN_Signs: return Positive;

            //------------------------------------------------------
            // trivial NEGATIVE signs ls < rs
            //------------------------------------------------------
        case ZP_Signs: return Negative;
        case NP_Signs: return Negative;
        case NZ_Signs: return Negative;

            //------------------------------------------------------
            // all positive signs
            //------------------------------------------------------
        case PP_Signs: return Natural::Compare(ln,rn);

            //------------------------------------------------------
            // all negative signs
            //------------------------------------------------------
        case NN_Signs: return Natural::Compare(rn,ln);
    }

    return __Zero__;
}
