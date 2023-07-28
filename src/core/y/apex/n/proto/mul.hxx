
//__________________________________________________________________
//
//! Mulitplication of two proto
//__________________________________________________________________
static inline Proto *Mul(const Proto &lhs,
                         const Proto &rhs,
                         MulProc      mul,
                         uint64_t    *ell)
{
    assert(0!=mul);
    return mul(lhs.block.entry,
               lhs.words,
               rhs.block.entry,
               rhs.words,
               ell);
}

//__________________________________________________________________
//
//! Multiplication of lhs by scalar rhs
//__________________________________________________________________
static inline Proto *Mul(const Proto   &lhs,
                         const uint64_t rhs)
{
    const  Splitter alias(rhs);
    return LongMul(lhs.block.entry,lhs.words,alias.w,alias.n,0);
}
