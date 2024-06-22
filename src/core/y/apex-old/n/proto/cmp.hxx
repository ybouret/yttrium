
//__________________________________________________________________
//
//
// Comparisons
//
//__________________________________________________________________

//__________________________________________________________________
//
//! check if lhs and rhs are different
//__________________________________________________________________
static inline bool AreDifferent(const Proto &lhs, const Proto &rhs) noexcept
{
    const size_t lwords = lhs.words;
    switch(Sign::Of(lwords,rhs.words))
    {
        case Negative:
        case Positive: return true;
        case __Zero__:
            break;
    }
    return 0 != memcmp(lhs.block.entry,rhs.block.entry,lwords*WordSize);
}

//__________________________________________________________________
//
//! check if lhs and rhs are equal
//__________________________________________________________________
static inline bool AreEqual(const Proto &lhs, const Proto &rhs) noexcept
{
    const size_t lwords = lhs.words;
    switch(Sign::Of(lwords,rhs.words))
    {
        case Negative:
        case Positive:
            return false;
        case __Zero__:
            break;
    }
    return 0 == memcmp(lhs.block.entry,rhs.block.entry,lwords*WordSize);
}

//__________________________________________________________________
//
//! default comparison
//__________________________________________________________________
static inline
SignType Compare(const WordType * const lhs, const size_t lnw,
                 const WordType * const rhs, const size_t rnw) noexcept
{
    assert(0!=lhs);
    assert(0!=rhs);
    if(lnw<rnw)
    {
        assert(rhs[rnw-1]>0);
        return Negative;
    }
    else
    {
        if(rnw<lnw)
        {
            assert(lhs[lnw-1]>0);
            return Positive;
        }
        else
        {
            assert(lnw==rnw);
            for(size_t i=lnw;i>0;)
            {
                const WordType L = lhs[--i];
                const WordType R = rhs[i];
                switch(Sign::Of(L,R))
                {
                    case Negative: return Negative;
                    case Positive: return Positive;
                    case __Zero__: continue;;
                }
            }
            return __Zero__;
        }
    }
}

//__________________________________________________________________
//
//! compare lhs,rhs
//__________________________________________________________________
static inline SignType Compare(const Proto &lhs, const Proto &rhs) noexcept
{
    return Compare(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words);
}

//__________________________________________________________________
//
//! compare lhs, uint
//__________________________________________________________________
static inline SignType Compare(const Proto &lhs, const uint64_t &rhs)
{
    const Splitter alias(rhs);
    return Compare(lhs.block.entry,lhs.words,alias.w,alias.n);
}

//__________________________________________________________________
//
//! compare uint, rhs
//__________________________________________________________________
static inline SignType Compare(const uint64_t lhs, const Proto &rhs)
{
    const Splitter alias(lhs);
    return Compare(alias.w,alias.n,rhs.block.entry,rhs.words);
    }
